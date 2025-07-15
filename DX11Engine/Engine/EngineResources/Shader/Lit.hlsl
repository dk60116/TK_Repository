// ───────────── 상수 버퍼
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
    uint useTexture;
    uint boneCount;
    float2 padding; // 16바이트 정렬
};

cbuffer PerBones : register(b3)
{
    float4x4 gBones[128];
};

// ───────────── 텍스처 및 샘플러
Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);
// ───────────── 버텍스 입력

#define MAX_LIGHTS 64

struct Light
{
    float3 position;
    float intensity;
    
    float direction;
    float spotAngle;
    
    float3 color;
    float range;
};

cbuffer PerLight : register(b4)
{
    Light gLight[MAX_LIGHTS];
    int gLightCount;
};

struct VSIn
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float2 uv : TEXCOORD0;
    float3 tangentL : TANGENT;
    uint4 boneIndices : BLENDINDICES;
    float4 boneWeights : BLENDWEIGHT;
};

// ───────────── 버텍스 출력
struct VSOut
{
    float4 posH : SV_POSITION;
    float3 normalW : NORMAL;
    float2 uv : TEXCOORD0;
};

// ───────────── 버텍스 셰이더
VSOut VSMain(VSIn v)
{
    VSOut o;
    
    // 스킨 포지션
    float4 skinnedPos = float4(v.posL, 1.0f);

    if (boneCount)
    {
        skinnedPos = float4(0, 0, 0, 0);

        [unroll]
        for (int i = 0; i < 4; ++i)
        {
            uint idx = v.boneIndices[i];
            float w = v.boneWeights[i];
            skinnedPos += mul(float4(v.posL, 1.0f), gBones[idx]) * w;
        }
    }

    // 스킨 노멀
    float3 skinnedNormal = v.normalL;

    if (boneCount)
    {
        skinnedNormal = float3(0, 0, 0);

        [unroll]
        for (int i = 0; i < 4; ++i)
        {
            uint idx = v.boneIndices[i];
            float w = v.boneWeights[i];
            skinnedNormal += mul((float3x3) gBones[idx], v.normalL) * w;
        }
    }

    // 위치 변환
    float4 posW = mul(skinnedPos, world);
    float4 posV = mul(posW, view);
    o.posH = mul(posV, proj);

    // 노멀 변환
    float3 normalW = mul((float3x3) world, skinnedNormal);
    o.normalW = normalize(normalW);

    o.uv = v.uv;

    return o;
}

// ───────────── 픽셀 셰이더
float4 PSMain(VSOut input) : SV_TARGET
{
    if (useTexture != 0)
    {
        float4 texColor = gTexture.Sample(gSampler, input.uv);
        return texColor * baseColor;
    }
    else
        return baseColor;
}