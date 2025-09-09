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

#define MAX_LIGHTS 64

#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

#pragma pack_matrix(row_major)
cbuffer PerLight : register(b4)
{
    float4x4 gLight[MAX_LIGHTS];
};

// 텍스처 & 샘플러
Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

texture2D gDiffuseTexture : register(t1);
texture2D gNormalTexture : register(t2);
texture2D gShadeTexture : register(t3);
texture2D gSpecularTexture : register(t4);

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
    return (useTexture != 0) ? gTexture.Sample(gSampler, i.uv) * baseColor : baseColor;
}