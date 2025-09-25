// 상수 버퍼
cbuffer PerObject : register(b0)
{
    float4x4 world;
};

cbuffer PerCamera : register(b1)
{
    float3 gPos;
    float padding1;

    float4x4 gView;
    float4x4 gProj;
    float4x4 gViewInv;
};

cbuffer PerMaterial : register(b2)
{
    float4 baseColor; // rgba 0~1
    uint useTexture;
};

// 텍스처 & 샘플러
Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

Texture2D gDiffuseTexture : register(t1);
Texture2D gShadingTexture : register(t4);

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
    
    o.posH = mul(mul(posW, gView), gProj);

    o.posW = posW.xyz;
    o.uv = v.uv;

    return o;
}

// 픽셀 셰이더
float4 PSMain(VSOut i) : SV_TARGET
{
    float4 albedo = gDiffuseTexture.Sample(gSampler, i.uv);
    
    if (albedo.a < 0.001f)
        discard;
    if (all(albedo.rgb == float3(1.0f, 0.0f, 1.0f)))
        discard;

    float3 shade = gShadingTexture.Sample(gSampler, i.uv).rgb;

    float3 lit = saturate(albedo.rgb * shade);

    return float4(lit, albedo.a);
}