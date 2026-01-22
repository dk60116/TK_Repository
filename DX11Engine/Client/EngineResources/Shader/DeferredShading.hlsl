// DeferredShading.hlsl
// Fullscreen lighting pass for GBuffer (Albedo + Normal + Depth).

cbuffer PerObject : register(b0)
{
    float4x4 world;
};

cbuffer PerCamera : register(b1)
{
    float3 camPos;
    float4x4 view;
    float4x4 proj;
    float cpadding;
};

#pragma pack_matrix(row_major)
cbuffer PerLight : register(b4)
{
    float4x4 gLight[64];
};

Texture2D gAlbedo : register(t0);
Texture2D gNormal : register(t1);
Texture2D gDepth : register(t2);
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

float3 DecodeNormal(float3 enc01)
{
    return normalize(enc01 * 2.0f - 1.0f);
}

float4 PSMain(VSOut i) : SV_Target
{
    float2 uv = i.uv;

    float3 albedo = gAlbedo.Sample(gSampler, uv).rgb;
    float3 N = DecodeNormal(gNormal.Sample(gSampler, uv).xyz);

    // Directional light (gLight[0])
    float lightType = gLight[0][3][0];
    float lightEnabled = gLight[0][3][2];
    if (lightType != 0.0f || lightEnabled <= 0.0f)
    {
        return float4(albedo, 1.0f);
    }

    float3 lightDir = float3(gLight[0][1][0], gLight[0][1][1], gLight[0][1][2]);
    float3 lightColor = float3(gLight[0][2][0], gLight[0][2][1], gLight[0][2][2]);
    float intensity = gLight[0][1][3];
    float ambientK = gLight[0][2][3];

    float3 L = normalize(-lightDir);
    float ndotl = saturate(dot(N, L));

    float3 ambient = lightColor * ambientK;
    float3 diffuse = lightColor * ndotl * intensity;

    float3 lit = albedo * saturate(ambient + diffuse);
    return float4(lit, 1.0f);
}
