// ShadowDepthPresent.hlsl
// 목적: ShadowDepth SRV(t0)를 그레이스케일로 표시

cbuffer PerObject : register(b0)
{
    float4x4 world;
};

cbuffer PerCamera : register(b1)
{
    float3 camPos; // 사용 안 함(바인딩 호환)
    float _pad0;
    float4x4 view;
    float4x4 proj;
};

Texture2D<float> gShadowDepth : register(t0);
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

float4 PSMain(VSOut i) : SV_Target
{
    float2 uv = i.uv;
    uv.y = 1.0f - uv.y;

    float d = gShadowDepth.Sample(gSampler, uv);

    // 보기 편하게 near를 밝게 보고 싶으면 반전:
    d = 1.0f - d;
    d = saturate(pow(d, 20.0f));

    d = saturate(d);
    return float4(d, d, d, 1.0f);
}
