// 상수 버퍼
cbuffer PerObject : register(b0)
{
    float4x4 world;
};

cbuffer PerCamera : register(b1)
{
    float3 pos;
    float4x4 view;
    float4x4 proj;
    float cpadding;
};

cbuffer PerMaterial : register(b2)
{
    float4 baseColor; // rgba 0~1
    uint useTexture;
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

// 텍스처 & 샘플러
SamplerState gSampler : register(s0);

Texture2D gNormalTexture : register(t2);

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
    
    o.posH = mul(mul(posW, view), proj);

    o.posW = posW.xyz;
    o.uv = v.uv;

    return o;
}

// 픽셀 셰이더
float4 PSMain(VSOut i) : SV_TARGET
{
    float3 N = normalize(gNormalTexture.Sample(gSampler, i.uv).xyz * 2.0f - 1.0f);

    float3 finalLight = float3(0, 0, 0);
    uint lightCount = (uint) gLight[0][3][3]; // 총 라이트 수 저장 위치

    for (uint iLight = 0; iLight < lightCount; ++iLight)
    {
        uint type = (uint) gLight[iLight][3][0];
        float3 lightDir = float3(gLight[iLight][1][0], gLight[iLight][1][1], gLight[iLight][1][2]);
        float intensity = gLight[iLight][1][3];
        float ambientK = gLight[iLight][2][3];

        float3 L = normalize((type == LIGHT_TYPE_DIRECTIONAL) ? -lightDir : lightDir);

        float shade = saturate(dot(N, L));
        float lit = saturate(ambientK + shade * intensity);

        finalLight += lit;
    }

    finalLight = saturate(finalLight);

    return float4(finalLight, 1.0f);
}