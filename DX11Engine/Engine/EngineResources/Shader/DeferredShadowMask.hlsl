// DeferredShadowMask.hlsl

#define MAX_LIGHTS 64
#define LIGHT_TYPE_DIRECTIONAL 0

cbuffer PerObject : register(b0)
{
    float4x4 world;
};

cbuffer PerCamera : register(b1)
{
    float3 camPos;
    float cpadding;
    float4x4 view;
    float4x4 proj;
};

#pragma pack_matrix(row_major)
cbuffer PerLight : register(b4)
{
    float4x4 gLight[MAX_LIGHTS];
};

cbuffer PerCustomValue : register(b5)
{
    float4x4 gInvViewProj;
    float4x4 gViewProj;
    float2 gShadowParams; // x = sample distance, y = depth bias
    float2 gShadowPadding;
};

Texture2D gNormal : register(t0);
Texture2D<float> gDepth : register(t1);
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
    float4 posW = mul(float4(v.posL, 1), world);
    float4 posV = mul(posW, view);
    o.posH = mul(posV, proj);
    o.uv = v.uv;
    return o;
}

float3 DecodeNormal(float3 enc01)
{
    return normalize(enc01 * 2.0f - 1.0f);
}

float3 ReconstructWorldPos(float2 uvScreen, float depth01)
{
    float2 ndc = float2(uvScreen.x * 2.0f - 1.0f, uvScreen.y * 2.0f - 1.0f);
    float4 clip = float4(ndc, depth01, 1.0f);
    float4 w = mul(clip, gInvViewProj);
    return w.xyz / max(w.w, 1e-6f);
}

float4 PSMain(VSOut i) : SV_Target
{
    float2 uvScreen = i.uv;
    float2 uvTex = float2(i.uv.x, 1.0f - i.uv.y);

    float depth01 = gDepth.SampleLevel(gSampler, uvTex, 0);
    if (depth01 >= 0.999999f)
        return float4(1, 1, 1, 1);

    float3 N = DecodeNormal(gNormal.Sample(gSampler, uvTex).xyz);
    float3 posW = ReconstructWorldPos(uvScreen, depth01);

    float3 lightDir = 0;
    bool hasLight = false;

    int lightCount = (int)gLight[0][3][3];
    [loop]
    for (int li = 0; li < lightCount; ++li)
    {
        if (gLight[li][3][2] < 0.5f)
            continue;

        uint lightType = (uint)gLight[li][3][0];
        if (lightType != LIGHT_TYPE_DIRECTIONAL)
            continue;

        lightDir = normalize(-float3(gLight[li][1][0], gLight[li][1][1], gLight[li][1][2]));
        hasLight = true;
        break;
    }

    if (!hasLight)
        return float4(1, 1, 1, 1);

    float sampleDistance = gShadowParams.x;
    float depthBias = gShadowParams.y;

    posW += N * depthBias;
    float3 samplePos = posW + lightDir * sampleDistance;

    float4 clip = mul(float4(samplePos, 1.0f), gViewProj);
    float2 projNdc = clip.xy / max(clip.w, 1e-6f);
    float2 projUv = projNdc * 0.5f + 0.5f;

    if (projUv.x < 0.0f || projUv.x > 1.0f || projUv.y < 0.0f || projUv.y > 1.0f)
        return float4(1, 1, 1, 1);

    float2 projTex = float2(projUv.x, 1.0f - projUv.y);
    float sampleDepth = gDepth.SampleLevel(gSampler, projTex, 0);
    float projDepth = clip.z / max(clip.w, 1e-6f);

    float shadow = (sampleDepth + depthBias < projDepth) ? 0.0f : 1.0f;
    return float4(shadow, shadow, shadow, 1.0f);
}
