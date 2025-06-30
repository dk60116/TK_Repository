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
    bool useTexture;
    float3 padding; // 16바이트 정렬
};

// ───────────── 텍스처 및 샘플러
Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

// ───────────── 버텍스 입력
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
    float2 uv : TEXCOORD0;
};

// ───────────── 버텍스 셰이더
VSOut VSMain(VSIn v)
{
    VSOut o;

    float4 posW = mul(float4(v.posL, 1.0f), world);
    float4 posV = mul(posW, view);
    o.posH = mul(posV, proj);

    o.uv = v.uv;
    return o;
}

// ───────────── 픽셀 셰이더
float4 PSMain(VSOut input) : SV_TARGET
{
    float4 texColor = gTexture.Sample(gSampler, input.uv);
    return useTexture ? texColor : baseColor;
}