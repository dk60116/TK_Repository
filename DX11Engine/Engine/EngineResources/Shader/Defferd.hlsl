// 상수 버퍼
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
    float4 baseColor; // rgba 0~1
    uint useTexture;
};

// 텍스처 & 샘플러
Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

// 버텍스 입출력
struct VSIn
{
    float3 posL : POSITION;
    float2 uv : TEXCOORD0;
};

struct VSOut
{
    float4 posH : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 posW : TEXCOORD1;
};

struct PS_OUT_BACKBUFFER
{
    float4 color : SV_TARGET0;
};

struct PS_OUT_LIGHT
{
    vector shade : SV_TARGET0;
    vector specular : SV_TARGET1;
};

// 버텍스 셰이더
VSOut VSMain(VSIn v)
{
    VSOut o;

    float4 posW = mul(float4(v.posL, 1.0f), world);
    
    o.posH = mul(mul(posW, view), proj);

    o.posW = posW.xyz;
    o.uv = v.uv;

    return o;
}

// 픽셀 셰이더
float4 PSMain(VSOut i) : SV_TARGET
{
    return (useTexture != 0) ? gTexture.Sample(gSampler, i.uv) : baseColor;
}