cbuffer PerCamera : register(b1)
{
    float3 gPos;
    float padding1;

    float4x4 gView;
    float4x4 gProj;
    float4x4 gViewInv;
};

#define MAX_LIGHTS 256
#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

#pragma pack_matrix(row_major)
cbuffer PerLight : register(b4)
{
    float4x4 gLight[MAX_LIGHTS];
};

Texture2D gGBuffer0 : register(t0);
Texture2D gGBuffer1 : register(t1);
Texture2D gGBuffer2 : register(t2);
SamplerState gSampler : register(s0);

struct VSIn
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float2 uv : TEXCOORD0;
    float3 tangentL : TANGENT;

    float4 instance_row0 : INSTANCE0;
    float4 instance_row1 : INSTANCE1;
    float4 instance_row2 : INSTANCE2;
    float4 instance_row3 : INSTANCE3;
};

struct VSOut
{
    float4 posH : SV_POSITION;
    float2 uv : TEXCOORD0;
};

VSOut VSMain(VSIn input)
{
    VSOut o;
    float2 pos = input.posL.xy * 2.0f;
    o.posH = float4(pos, 0.0f, 1.0f);
    o.uv = input.posL.xy + 0.5f;
    return o;
}

float4 PSMain(VSOut input) : SV_TARGET
{
    float4 rt0 = gGBuffer0.Sample(gSampler, input.uv);
    float4 rt1 = gGBuffer1.Sample(gSampler, input.uv);
    float4 rt2 = gGBuffer2.Sample(gSampler, input.uv);

    float3 albedo = rt0.rgb;
    float3 normal = normalize(rt1.rgb * 2.0f - 1.0f);
    float smoothness = rt1.a;
    float3 posW = rt2.xyz;

    float3 diffuseSum = float3(0, 0, 0);
    float3 specularSum = float3(0, 0, 0);

    int lightCount = (int)gLight[0][3][3];
    float3 viewDir = normalize(gPos - posW);

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

        float3 L = 0;
        float atten = 1.0f;

        if (lightType == LIGHT_TYPE_DIRECTIONAL)
        {
            L = normalize(-lightDir);
        }
        else
        {
            float3 dirToLight = lightPos - posW;
            float dist = length(dirToLight);
            if (dist > range)
                continue;
            L = dirToLight / max(dist, 0.0001f);
            atten = 1.0f / (1.0f + attenuationK * dist * dist);
        }

        float NdotL = saturate(dot(normal, L));
        float3 diffuse = albedo * lightColor * NdotL * intensity * atten;

        float3 H = normalize(L + viewDir);
        float specPower = lerp(8.0f, 128.0f, smoothness);
        float spec = pow(saturate(dot(normal, H)), specPower) * intensity * atten;

        diffuseSum += diffuse + albedo * ambientK;
        specularSum += spec * lightColor;
    }

    return float4(diffuseSum + specularSum, 1.0f);
}
