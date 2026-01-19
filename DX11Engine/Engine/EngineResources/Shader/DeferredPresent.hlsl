// DeferredPresent.hlsl
// 목적: SRV(t0) 텍스처를 그대로 출력 (디퍼드 디버그 썸네일/프레젠트 용)
// 필요 최소: b0(world), b1(view/proj), POSITION/UV, t0/s0

cbuffer PerObject : register(b0)
{
    float4x4 world;
};

cbuffer PerCamera : register(b1)
{
    float3 pos; // 사용 안 함(엔진 바인딩 호환)
    float4x4 view;
    float4x4 proj;
    float cpadding;
};

Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

struct VSIn
{
    float3 posL : POSITION;
    float2 uv : TEXCOORD0;
};

struct VSOut
{
    float4 posH : SV_POSITION;
    float2 uv : TEXCOORD0;
};

VSOut VSMain(VSIn v)
{
    VSOut o;
    float4 posW = mul(float4(v.posL, 1.0f), world);
    float4 posV = mul(posW, view);
    o.posH = mul(posV, proj);
    o.uv = v.uv;
    return o;
}

float4 PSMain(VSOut input) : SV_Target
{
    return gTexture.Sample(gSampler, input.uv);
}