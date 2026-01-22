cbuffer PerObject : register(b0)
{
    float4x4 world;
};

cbuffer PerCamera : register(b1)
{
    float3 pos;
    float _pad0;
    float4x4 view;
    float4x4 proj;
};

cbuffer PerMaterial : register(b2)
{
    float4 baseColor;
    uint useTexture;
    uint boneCount;
    float2 padding;
};

cbuffer PerLight : register(b4)
{
    float4x4 gLights[64];
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

VSOut VSMain(VSIn v)
{
    VSOut o;
    float4 posW = mul(float4(v.posL, 1.0f), world);
    float4 posV = mul(posW, view);
    o.posH = mul(posV, proj);
    o.uv = v.uv;
    return o;
}

float3 DecodeNormal(float3 enc)
{
    return normalize(enc * 2.0f - 1.0f);
}

float3 ReconstructViewPos(float2 uv, float depth)
{
    float2 ndc = float2(uv.x * 2.0f - 1.0f, uv.y * 2.0f - 1.0f);

    float c = proj._33;
    float d = proj._34;
    float e = proj._43;
    float f = proj._44;

    float denom = depth * d - c;
    float safeDenom = (abs(denom) < 1e-5f) ? (denom < 0.0f ? -1e-5f : 1e-5f) : denom;
    float zView = (e - depth * f) / safeDenom;

    float wView = zView * d + f;
    float xView = ndc.x * wView / proj._11;
    float yView = ndc.y * wView / proj._22;

    return float3(xView, yView, zView);
}

float3 ApplyLight(float3 albedo, float3 normalV, float3 viewPos, float4x4 lightData)
{
    float3 lightPosW = lightData[0].xyz;
    float range = lightData[0].w;

    float3 lightDirW = lightData[1].xyz;
    float spotCos = lightData[1].w;

    float3 lightColor = lightData[2].xyz;
    float lightType = lightData[3].x;
    float attenuation = lightData[3].y;
    float enabled = lightData[3].z;

    if (enabled < 0.5f)
        return 0.0f;

    float3 result = 0.0f;

    float3 lightPosV = mul(float4(lightPosW, 1.0f), view).xyz;
    float3 lightDirV = normalize(mul(lightDirW, (float3x3)view));

    if (lightType < 0.5f)
    {
        float3 L = normalize(-lightDirV);
        float NdotL = saturate(dot(normalV, L));
        result = albedo * lightColor * NdotL;
    }
    else
    {
        float3 toLight = lightPosV - viewPos;
        float dist = length(toLight);
        if (dist < range)
        {
            float3 L = toLight / max(dist, 1e-4f);
            float NdotL = saturate(dot(normalV, L));

            float att = 1.0f / (1.0f + attenuation * dist * dist);
            att *= saturate(1.0f - dist / range);

            if (lightType > 1.5f)
            {
                float spot = saturate((dot(-lightDirV, L) - spotCos) / max(1.0f - spotCos, 1e-4f));
                att *= spot;
            }

            result = albedo * lightColor * NdotL * att;
        }
    }

    return result;
}

float4 PSMain(VSOut input) : SV_Target
{
    float2 uv = input.uv;
    uv.y = 1.0f - uv.y;

    float4 albedoSample = gAlbedo.Sample(gSampler, uv);
    float3 normalW = DecodeNormal(gNormal.Sample(gSampler, uv).xyz);
    float depth = gDepth.SampleLevel(gSampler, uv, 0);

    float3 normalV = normalize(mul(normalW, (float3x3)view));
    float3 viewPos = ReconstructViewPos(uv, depth);

    int lightCount = (int)gLights[0][3][3];
    lightCount = clamp(lightCount, 0, 64);

    float ambient = gLights[0][2][3];
    float3 color = albedoSample.rgb * ambient;

    for (int i = 0; i < lightCount; ++i)
    {
        color += ApplyLight(albedoSample.rgb, normalV, viewPos, gLights[i]);
    }

    return float4(saturate(color), albedoSample.a);
}
