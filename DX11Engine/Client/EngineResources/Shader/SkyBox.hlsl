// ───────────── 상수 버퍼
cbuffer PerObject : register(b0)
{
    float4x4 world;
};

cbuffer PerCamera : register(b1)
{
    float3 pos;
    float4x4 view;
    float4x4 proj;
    float cpadding;
};

cbuffer PerMaterial : register(b2)
{
    float4 baseColor;
    uint useTexture;
    float mpadding;
};

// ───────────── 텍스처 및 샘플러
Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);
// ───────────── 버텍스 입력

struct VSIn
{
    float3 posL : POSITION;
    float2 uv   : TEXCOORD0;
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

    float4 pos = float4(v.posL, 1.0f);

    float4x4 viewNoTrans = view;
    viewNoTrans._41 = 0;
    viewNoTrans._42 = 0;
    viewNoTrans._43 = 0;

    float4 posV = mul(pos, viewNoTrans);
    float4 posP = mul(posV, proj);

    o.posH = posP; // 반드시 설정 필요
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