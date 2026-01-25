// ShadowMask.hlsl (example)
// 목적: ShadowMask RT에 0~1 shadow factor를 기록

cbuffer PerObject : register(b0)
{
    float4x4 world; // fullscreen rect용
};

cbuffer PerCamera : register(b1)
{
    float3 camPos; // 사용 안 해도 바인딩 호환용
    float _pad0;
    float4x4 view;
    float4x4 proj;
};

cbuffer PerCustomValue : register(b5)
{
    float4x4 gInvViewProj; // camera invVP
};

cbuffer PerShadow : register(b6)
{
    float4x4 gLightViewProj; // lightVP
    float3 gLightDirWS;
    float gBiasBase;
    float2 gShadowTexel; // 1/shadowMapSize
    float gStrength;
    float _pad1;
};

Texture2D<float> gDepth : register(t0); // camera depth
Texture2D<float> gShadowDepth : register(t1); // shadow depth map (light)
Texture2D gNormal : register(t2); // optional (없으면 제거)
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

// ---------- helpers ----------
float3 ReconstructWorldPos(float2 uv, float depth01)
{
    // D3D NDC: x,y = -1..1, z = 0..1
    float x = uv.x * 2.0f - 1.0f;
    float y = (1.0f - uv.y) * 2.0f - 1.0f; // uv 원점(top-left) 기반이면 보통 이렇게
    float4 p = float4(x, y, depth01, 1.0f);

    float4 w = mul(p, gInvViewProj);
    return w.xyz / w.w;
}

float3 DecodeNormalWS(float4 n) // 당신 GBuffer 규약에 맞게 수정
{
    // 예시: 0..1 저장 -> -1..1 복원 (월드 노멀이라고 가정)
    float3 nn = n.xyz * 2.0f - 1.0f;
    return normalize(nn);
}

float ComputeShadow(float3 posWS, float3 normalWS)
{
    float4 posLS = mul(float4(posWS, 1.0f), gLightViewProj);
    posLS.xyz /= posLS.w;

    float2 suv;
    suv.x = posLS.x * 0.5f + 0.5f;
    suv.y = -posLS.y * 0.5f + 0.5f; // 이 부분은 엔진 좌표계에 따라 flip 필요

    if (suv.x < 0 || suv.x > 1 || suv.y < 0 || suv.y > 1)
        return 1.0f;

    float current = posLS.z; // 0..1

    float ndotl = saturate(dot(normalWS, -gLightDirWS));
    float bias = max(gBiasBase, gBiasBase * (1.0f - ndotl));

    // PCF 3x3
    float sum = 0.0f;
    [unroll]
    for (int y = -1; y <= 1; ++y)
    {
        [unroll]
        for (int x = -1; x <= 1; ++x)
        {
            float2 o = float2(x, y) * gShadowTexel;
            float d = gShadowDepth.Sample(gSampler, suv + o);
            sum += (current - bias <= d) ? 1.0f : 0.0f;
        }
    }
    float s = sum / 9.0f;

    return lerp(1.0f - gStrength, 1.0f, s);
}

float4 PSMain(VSOut i) : SV_Target
{
    float2 uv = i.uv;

    float depth01 = gDepth.Sample(gSampler, uv);
    if (depth01 >= 0.9999f)
        return float4(1, 1, 1, 1);

    float3 posWS = ReconstructWorldPos(uv, depth01);

    float3 normalWS = float3(0, 1, 0);
    // normal 사용하려면 gNormal SRV 바인딩 + DecodeNormalWS 수정
    // normalWS = DecodeNormalWS(gNormal.Sample(gSampler, uv));

    float shadow = ComputeShadow(posWS, normalWS);
    return float4(shadow, shadow, shadow, 1.0f);
}
