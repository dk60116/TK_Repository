# DX11Engine Shadow Rendering Error Analysis

This document summarizes likely causes of shadow rendering errors observed in the DX11Engine
shadow pipeline. The focus is on code-paths used by `RenderShadowDepth` and `RenderShadowMask`.

## 0) Pipeline overview (for context)
- Shadow depth is rendered by `CCamera::RenderShadowDepth` into the ShadowDepth DSV at
  `LightSettings.shadowMapSize` resolution, using `ShadowDepth.hlsl` and the directional light’s
  view/projection derived from the camera frustum. The light’s view/proj are stored on the
  light and in `m_vLightViewProj` for later use.
- Shadow mask is built by `CCamera::RenderShadowMask` as a fullscreen quad, reading the scene
  depth, shadow depth SRV, and (optionally) normal buffer, and writing a 0–1 shadow factor.

## 1) Matrix packing mismatch in ShadowMask
`ShadowMask.hlsl` originally applied `#pragma pack_matrix(row_major)` globally, so all matrices
were interpreted as **row-major** in the shader. However, the engine’s material binding path
**transposes** the world/view/projection matrices before uploading them to the GPU
(`Bind_Matrix` and `Bind_Camera`). This means the fullscreen quad shader would receive
`world/view/proj` that were effectively transposed **and then** treated as row-major in HLSL,
which results in an unintended double-mismatch.

**Why this matters:**
- The shadow mask is rendered with a fullscreen quad (`RenderShadowMask`) and uses the
  `world/view/proj` matrices in `ShadowMask.hlsl`’s vertex shader. If those matrices are
  transposed but interpreted as row-major, the quad can be warped or positioned incorrectly,
  leading to missing or distorted shadow results.
- The same shader also relies on `gInvViewProj` and `gLightViewProj` that are **not**
  transposed by `Bind_Camera`, which means the shader is mixing conventions: row-major matrices
  for `gInvViewProj/gLightViewProj`, but transposed (column-major) matrices for
  `world/view/proj`.

**References:**
- `ShadowMask.hlsl` uses `#pragma pack_matrix(row_major)` and multiplies with `world/view/proj`.
- `CMaterial::Bind_Matrix` / `Bind_Camera` transpose matrices prior to upload.
- `CCamera::RenderShadowMask` uses `Bind_Matrix` / `Bind_Camera` for the fullscreen quad and
  separately uploads `gInvViewProj` and `ShadowCB` without transposes.

## 2) Normal-based bias is disabled (hardcoded normal)
`ShadowMask.hlsl` previously used a **hardcoded normal** (`float3 normalWS = float3(0,1,0)`) for
bias calculation instead of sampling the normal buffer. This means the shadow bias was
independent of actual surface orientation.

**Why this matters:**
- Any surface not aligned with the hardcoded normal will get incorrect biasing. This can
  manifest as acne on steep angles or over-aggressive peter-panning on shallow angles.
- The shader explicitly comments that normal decoding is disabled, so bias does not adapt per
  surface.

## 3) Depth bias settings can fight each other
There are **two** bias paths active at once:
- A fixed **rasterizer depth bias** in `m_pShadowRS` (DepthBias = 1000, SlopeScaledDepthBias = 1.0).
- A **shader comparison bias** in `ShadowMask.hlsl` (`gBiasBase` plus a slope-based term from N·L).

These can compound and lead to either over-bias (peter-panning) or under-bias (shadow acne),
especially when combined with the hardcoded normal from section 2.

## 4) Shadow map size vs. texel size mismatch risk
The shadow map resolution comes from `SceneManager::LightSettings.shadowMapSize`, which is
used in **two different places**:
- It determines the **texture resolution** when `CRenderTargetManager::CreateTargets` builds
  the ShadowDepth render target.
- It is also used at runtime to compute `gShadowTexel` in `RenderShadowMask` for PCF offsets.

If the shadow map size changes **after initialization**, the shadow depth render target is
not automatically re-created, but `gShadowTexel` will still use the new size. This mismatch
leads to sampling the shadow map with incorrect texel offsets and can blur or distort the
shadow comparison.

## 5) Y-flip handling can be inconsistent
`ShadowMask.hlsl` flips UVs when reconstructing world position and also flips the shadow
projection’s Y coordinate. This is often correct, but it assumes that all upstream UV and
texture coordinate conventions match. If the fullscreen quad’s UV origin or the depth SRV
projection differs, a double flip can occur, leading to vertically flipped shadows.

This is more subtle but should be checked in tandem with (1) because any matrix packing issue
can make Y-flip logic appear incorrect.

---

## Suggested investigation order
1. **Verify matrix packing for ShadowMask**: ensure the shader packing convention matches how
   matrices are uploaded (row-major vs. transposed). Validate by drawing the shadow mask debug
   output and checking for full-screen coverage.
2. **Use per-pixel normals** in `ShadowMask.hlsl` and compare acne/peter-panning on sloped
   surfaces, then re-tune `gBiasBase` and/or the rasterizer bias.
3. **Audit shadow map resolution changes**: ensure `CRenderTargetManager::Resize` or a similar
   path recreates `ShadowDepth` when `shadowMapSize` changes.
4. **Validate UV flip** behavior by comparing depth/debug passes (ShadowDepthPresent vs.
   ShadowMask).
