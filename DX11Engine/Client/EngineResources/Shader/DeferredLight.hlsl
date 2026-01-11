cbuffer PerCamera : register(b1)
{
    float3 camPos;
    float4x4 view;
    float4x4 proj;
    float cpadding;
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

Texture2D gAlbedo : register(t0);
Texture2D gNormal : register(t1);
Texture2D gPosition : register(t2);
SamplerState gSampler : register(s0);

struct VSIn
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float2 uv : TEXCOORD0;
    float3 tangentL : TANGENT;
};

struct VSOut
{
    float4 posH : SV_POSITION;
    float2 uv : TEXCOORD0;
};

VSOut VSMain(VSIn v)
{
    VSOut o;
    float2 pos = v.posL.xy * 2.0f;
    o.posH = float4(pos.x, -pos.y, 0.0f, 1.0f);
    o.uv = v.uv;
    return o;
}

float4 PSMain(VSOut input) : SV_TARGET
{
    float4 albedo = gAlbedo.Sample(gSampler, input.uv);
    float3 normalW = normalize(gNormal.Sample(gSampler, input.uv).xyz * 2.0f - 1.0f);
    float3 posW = gPosition.Sample(gSampler, input.uv).xyz;

    float3 V = normalize(camPos - posW);

    float3 diffuseSum = float3(0, 0, 0);
    float3 ambientSum = float3(0, 0, 0);
    float3 specularSum = float3(0, 0, 0);

    int lightCount = (int)gLight[0][3][3];

    for (int i = 0; i < lightCount; ++i)
    {
        if (gLight[i][3][2] < 0.5)
            continue;

        uint lightType = (uint)gLight[i][3][0];
        float3 lightPos = float3(gLight[i][0][0], gLight[i][0][1], gLight[i][0][2]);
        float3 lightDir = float3(gLight[i][1][0], gLight[i][1][1], gLight[i][1][2]);
        float3 lightColor = float3(gLight[i][2][0], gLight[i][2][1], gLight[i][2][2]);
        float intensity = gLight[i][1][3];
        float range = gLight[i][0][3];
        float attenuationK = gLight[i][3][1];
        float ambientK = gLight[i][2][3];

        float3 L;
        float attenuation = 1.0f;

        if (lightType == LIGHT_TYPE_DIRECTIONAL)
        {
            L = normalize(-lightDir);
        }
        else if (lightType == LIGHT_TYPE_POINT)
        {
            float3 toLight = lightPos - posW;
            float dist = length(toLight);
            L = toLight / dist;
            attenuation = saturate(1.0f - dist / range) * attenuationK;
        }
        else
            continue;

        float NdotL = saturate(dot(normalW, L));
        float3 diffuse = lightColor * NdotL * intensity * attenuation;
        diffuseSum += diffuse;

        float3 R = reflect(-L, normalW);
        float RdotV = saturate(dot(R, V));
        float3 specular = lightColor * pow(RdotV, 50.0f) * intensity * attenuation;
        specularSum += specular;

        float3 ambient = lightColor * ambientK;
        ambientSum += ambient;
    }

    diffuseSum = max(diffuseSum, float3(0.1f, 0.1f, 0.1f));
    float3 litDiffuse = albedo.rgb * saturate(ambientSum + diffuseSum);
    float3 finalColor = saturate(litDiffuse + specularSum);

    return float4(finalColor, albedo.a);
}
