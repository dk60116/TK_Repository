// ─────────────────────────────────────────────────────────
// 0) 옵션 : define 로 스키닝 Variant 선택
//  - 엔진에서 SkinnedMeshRenderer 렌더링 시   #define GPU_SKINNING
//  - 일반 MeshRenderer             렌더링 시   #undef  GPU_SKINNING
// ─────────────────────────────────────────────────────────

#define MAX_BONES 128
// ─────────────────────────────────────────────────────────

// ── b0, b1, b2 : 기존과 동일
cbuffer PerObject : register(b0)
{
    float4x4 world;
};
cbuffer PerCamera : register(b1)
{
    float4x4 view;
    float4x4 proj;
};
cbuffer PerMaterial : register(b2)
{
    float4 baseColor;
    bool useTexture;
    float3 _pad__;
};

cbuffer Bones : register(b3){
    float4x4 boneMatrices[MAX_BONES];
};

// ─────────────────────────────────────────────────────────
Texture2D diffuseMap : register(t0);
SamplerState samLinear : register(s0);

// ── 정점 구조체
struct VSIn
{
    float3 posL : POSITION;
    float2 texcoord : TEXCOORD0;

    uint4  boneIdx  : BLENDINDICES0;
    float4 boneWgt  : BLENDWEIGHT0;
};

struct VSOut
{
    float4 posH : SV_POSITION;
    float2 uv : TEXCOORD0;
};

// ───────────────────────────
// 1) Skinning 헬퍼
// ───────────────────────────

float4x4 GetSkinMatrix(uint index)
{
    return boneMatrices[index];
}

float4 SkinPos(float3 pos, uint4 idx, float4 w)
{
    float4 p0 = mul(float4(pos,1), GetSkinMatrix(idx.x)) * w.x;
    float4 p1 = mul(float4(pos,1), GetSkinMatrix(idx.y)) * w.y;
    float4 p2 = mul(float4(pos,1), GetSkinMatrix(idx.z)) * w.z;
    float4 p3 = mul(float4(pos,1), GetSkinMatrix(idx.w)) * w.w;
    return p0 + p1 + p2 + p3;
}
// ───────────────────────────
// 2) Vertex Shader
// ───────────────────────────
VSOut VSMain(VSIn v)
{
    VSOut o;

    float4 skinned = SkinPos(v.posL, v.boneIdx, v.boneWgt);
    float4 worldPos = mul(skinned, world);

    float4 viewPos = mul(worldPos, view);
    o.posH = mul(viewPos, proj);
    o.uv = v.texcoord;
    return o;
}

// ───────────────────────────
// 3) Pixel Shader
// ───────────────────────────
float4 PSMain(VSOut i) : SV_TARGET
{
    return useTexture ? diffuseMap.Sample(samLinear, i.uv) : baseColor;
}

// ───────────────────────────
// 4) Technique
// ───────────────────────────
technique11 UnlitTexture
{
    pass P0
    { // Static / Skinned 공용 pass
        SetVertexShader(CompileShader(vs_5_0, VSMain()));
        SetPixelShader(CompileShader(ps_5_0, PSMain()));
    }
}
