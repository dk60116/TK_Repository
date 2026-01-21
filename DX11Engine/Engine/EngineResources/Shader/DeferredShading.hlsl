// DeferredLighting_DiffuseOnly.hlsl

// 라이트 정의
#define MAX_LIGHTS 64

#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

cbuffer PerObject : register(b0)
{
    float4x4 world; // fullscreen quad용 (그대로 유지)
};

cbuffer PerCamera : register(b1)
{
    float3 camPos; // 사용 안 함(바인딩 호환)
    float4x4 view; // 사용 안 함
    float4x4 proj; // 사용 안 함
    float cPadding;
};


#pragma pack_matrix(row_major)
cbuffer PerLight : register(b4)
{
    float4x4 gLight[64];
};

cbuffer PerCustomValue : register(b10)
{
    float4x4 gInvViewProj;
};

Texture2D gAlbedo : register(t0);
Texture2D gNormal : register(t1);
Texture2D<float> gDepth : register(t2);
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

float3 ReconstructWorldPos(float2 uv, float depth01)
{
    // D3D NDC: x,y = [-1,1], z = [0,1]
    float4 ndc;
    ndc.x = uv.x * 2.0f - 1.0f;
    ndc.y = uv.y * 2.0f - 1.0f;
    ndc.z = depth01;
    ndc.w = 1.0f;

    float4 worldH = mul(ndc, gInvViewProj);
    worldH.xyz /= max(1e-6f, worldH.w);
    return worldH.xyz;
}

VSOut VSMain(VSIn v)
{
    VSOut o;
    float4 posW = mul(float4(v.posL, 1), world);
    // view/proj를 안 쓰려면 world에서 이미 clip-space로 만들거나,
    // 기존 Present처럼 view/proj를 Identity/Ortho로 넣어도 됩니다.
    // 여기서는 기존 Camera::RenderRTDebugDisplay 흐름 그대로(view/proj 바인딩) 사용을 전제로 둡니다.
    float4 posV = mul(posW, view);
    o.posH = mul(posV, proj);
    o.uv = v.uv;
    return o;
}

float3 DecodeNormal(float3 enc01)
{
    return normalize(enc01 * 2.0f - 1.0f);
}

float4 PSMain(VSOut i) : SV_Target
{
    float2 uv = i.uv;
    uv.y = 1.0f - uv.y;

    float3 albedo = gAlbedo.Sample(gSampler, uv).rgb;
    float3 N = DecodeNormal(gNormal.Sample(gSampler, uv).xyz);

    float d = gDepth.SampleLevel(gSampler, uv, 0);
    if (d >= 0.999999f) // sky/empty
        return float4(0, 0, 0, 1);

    float3 posW = ReconstructWorldPos(uv, d);

    float3 diffuseSum = 0;
    float3 ambientSum = 0;

    int lightCount = (int) gLight[0][3][3];

    [loop]
    for (int li = 0; li < lightCount; ++li)
    {
        if (gLight[li][3][2] < 0.5f)
            continue;

        uint lightType = (uint) gLight[li][3][0];
        float3 lightPos = float3(gLight[li][0][0], gLight[li][0][1], gLight[li][0][2]);
        float3 lightDir = float3(gLight[li][1][0], gLight[li][1][1], gLight[li][1][2]);
        float3 lightCol = float3(gLight[li][2][0], gLight[li][2][1], gLight[li][2][2]);
        float intensity = gLight[li][1][3];
        float range = gLight[li][0][3];
        float attenK = gLight[li][3][1];
        float ambientK = gLight[li][2][3];

        float3 L = 0;
        float att = 1.0f;

        if (lightType == LIGHT_TYPE_DIRECTIONAL)
        {
            L = normalize(-lightDir);
            att = 1.0f;
        }
        else if (lightType == LIGHT_TYPE_POINT)
        {
            float3 toLight = lightPos - posW;
            float dist = length(toLight);
            if (dist > range)
                continue;

            L = toLight / max(1e-6f, dist);
            att = saturate(1.0f - dist / range) * attenK;
        }
        else
        {
            continue;
        }

        float ndotl = saturate(dot(N, L));
        diffuseSum += lightCol * ndotl * intensity * att;
        ambientSum += lightCol * ambientK;
    }

    float3 lit = albedo * saturate(ambientSum + diffuseSum);
    return float4(saturate(lit), 1);
}
