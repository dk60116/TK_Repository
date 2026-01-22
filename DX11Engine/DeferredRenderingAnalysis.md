# Deferred Rendering 정밀 분석

## 사용한 터미널 명령
- `rg -n "Deferred"`
- `rg -n "GBuffer|Deferred" DX11Engine/Engine/Code`
- `rg -n "GBuffer|DeferredShading|DeferredPresent|Shading" DX11Engine/Engine/Code`
- `rg -n "G_BufferLit|GBufferLit|Deferred" DX11Engine/Engine/Code -g"*.cpp"`
- `rg -n "DeferredPresent" DX11Engine/Engine`
- `rg -n "To_LightInfo|gLight" DX11Engine/Engine/Code DX11Engine/Engine/Header`
- `sed -n '1,200p' DX11Engine/Engine/Code/Camera.cpp`
- `sed -n '240,560p' DX11Engine/Engine/Code/Camera.cpp`
- `sed -n '560,760p' DX11Engine/Engine/Code/Camera.cpp`
- `sed -n '1300,1420p' DX11Engine/Engine/Code/Resources.cpp`
- `sed -n '1,220p' DX11Engine/Engine/Code/RenderTargetManager.cpp`
- `sed -n '1,260p' DX11Engine/Engine/Code/RenderTarget.cpp`
- `sed -n '1,220p' DX11Engine/Engine/Code/Scene.cpp`
- `sed -n '1,220p' DX11Engine/Engine/Code/Renderer.cpp`
- `sed -n '1,200p' DX11Engine/Engine/Code/MeshRenderer.cpp`
- `sed -n '130,260p' DX11Engine/Engine/Code/SkinnedMeshRenderer.cpp`
- `sed -n '1,220p' DX11Engine/Engine/Code/Light.cpp`
- `sed -n '1,200p' DX11Engine/Engine/Header/RenderTarget.h`
- `sed -n '1,200p' DX11Engine/Engine/EngineResources/Shader/DeferredShading.hlsl`
- `sed -n '1,200p' DX11Engine/Engine/EngineResources/Shader/DeferredPresent.hlsl`
- `sed -n '1,200p' DX11Engine/Engine/EngineResources/Shader/DepthPresent.hlsl`
- `sed -n '1,220p' DX11Engine/Engine/EngineResources/Shader/GbufferLit.hlsl`

## 파이프라인 개요
- 렌더 타깃 구성은 `Albedo`, `Normal`, `Depth`, `Shading` 4종으로 고정되어 있습니다.
- 화면 해상도 기준으로 `Albedo`(R8G8B8A8_UNORM), `Normal`(R16G16B16A16_FLOAT), `Depth`(D24S8 + SRV), `Shading`(R16G16B16A16_FLOAT)을 생성합니다.
- 프레임 렌더링은 Scene의 `Render_Game()`에서 **GBuffer 패스 → Lighting 패스(Shading RT) → 백버퍼/UI/디버그 오버레이** 순서로 진행됩니다.

## 1) GBuffer 패스
- `CRenderTargetManager::Bind_GBuffer()`가 MRT로 Albedo/Normal RT와 Depth DSV를 바인딩합니다.
- `CRenderTargetManager::Clear_GBuffer()`는 Albedo=0, Normal=(0.5,0.5,1,1), Depth=1로 초기화합니다.
- 메쉬 렌더러는 기본 머티리얼로 `G_BufferLit (Material)`을 사용하며, GBuffer용 셰이더를 통해 `SV_Target0`(Albedo)와 `SV_Target1`(Normal)을 출력합니다.

### GBuffer 셰이더 동작 (GbufferLit.hlsl)
- 머티리얼 상수 버퍼에서 `baseColor`, 텍스처 사용 플래그, 노멀맵 사용 플래그 등을 받아 알베도 색을 산출합니다.
- Tangent/Bitangent/Normal을 기반으로 TBN을 구성해 노멀맵을 월드 노멀로 변환하며, 최종 월드 노멀을 0~1 범위로 인코딩해 출력합니다.
- 스키닝 경로에서는 본 행렬(최대 128개)을 적용해 위치/노멀/탄젠트를 변형합니다.

## 2) Lighting 패스 (DeferredShading)
- `CCamera::RenderLightingPass_ToShading()`가 `Shading` RT를 바인딩하고 풀스크린 쿼드를 렌더합니다.
- 이때 PS의 `t0/t1/t2`에 Albedo, Normal, Depth SRV를 바인딩합니다.
- `DeferredShading.hlsl`은 다음 구조로 조명 계산을 수행합니다.
  - `gAlbedo`, `gNormal`, `gDepth`를 샘플링.
  - `gLight[0]`을 **Directional light**로 가정하고, 타입이 0(방향광)이며 활성화된 경우에만 조명 계산.
  - 결과는 `albedo * (ambient + diffuse)`만 사용하는 **Diffuse Only** 조명 모델.
- 라이트 데이터는 `CLight::To_LightInfo()`에서 4x4 행렬 형태로 구성되며, 타입/활성화/강도/색/방향/ambient 등을 전달합니다.

## 3) BackBuffer 합성 및 디버그 출력
- Lighting 패스 결과는 `Shading` RT에 저장되지만, 현재 코드에서는 이 결과를 백버퍼에 합성하지 않습니다.
- 대신 `CCamera::RenderRTDebugDisplay()`에서 Albedo/Normal/Depth/Shading을 디버그 썸네일로 그려 확인할 수 있습니다.
- 디버그 출력용 셰이더:
  - `DeferredPresent.hlsl`: 일반 컬러 텍스처를 화면에 그대로 출력(수직 플립 포함).
  - `DepthPresent.hlsl`: Depth를 샘플링해 회색조로 시각화.

## 정리: 현재 구현의 특징과 한계
- **특징**: 단일 Directional light 기반의 Diffuse-Only Deferred Lighting 구성이며, GBuffer에는 Albedo/Normal만 저장합니다. Depth는 SRV로 샘플링 가능한 D24S8 텍스처로 유지합니다.
- **한계**: Lighting 패스 결과가 실제 게임 화면(백버퍼)로 합성되지 않아, 디버그 UI로만 확인 가능한 구조입니다. 이는 추후 `Shading` RT를 백버퍼에 그리는 Present/Composite 패스가 추가되어야 완전한 Deferred 파이프라인이 됩니다.
