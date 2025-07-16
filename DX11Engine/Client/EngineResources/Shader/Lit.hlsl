//────────────────── 상수 버퍼 ──────────────────
cbuffer PerObject : register(b0)
{
    float4x4 world;
};

cbuffer PerCamera : register(b1)
{
    float4x4 view;
    float4x4 proj;
};

cbuffer PerMaterial : register(b2)
{
    float4 baseColor; // rgba 0~1
    uint useTexture;
    uint boneCount;
    float2 padding; // 16B alignment
};

cbuffer PerBones : register(b3)
{
    float4x4 gBones[128];
};

//────────────────── 라이트 정의 ──────────────────
#define MAX_LIGHTS 64
#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT       1
// #define LIGHT_TYPE_SPOT     2   // 확장용

struct Light
{
    float3 position;
    float intensity; // 16B 슬롯 0

    float3 direction;
    float spotAngle; // 16B 슬롯 1 (direction 단위벡터)

    float3 color;
    float range; // 16B 슬롯 2

    uint type;
    float3 _pad; // 16B 슬롯 3 (패딩)
};

cbuffer PerLight : register(b4)
{
    Light gLight[MAX_LIGHTS];
    int gLightCount;
    int3 _padL; // 16B 정렬
};

//────────────────── 텍스처 & 샘플러 ──────────────────
Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

//────────────────── 버텍스 입출력 ──────────────────
struct VSIn
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float2 uv : TEXCOORD0;
    float3 tangentL : TANGENT;
    uint4 boneIndices : BLENDINDICES;
    float4 boneWeights : BLENDWEIGHT;
};

struct VSOut
{
    float4 posH : SV_POSITION;
    float3 normalW : NORMAL;
    float3 posW : TEXCOORD1;
    float2 uv : TEXCOORD0;
};

//────────────────── 버텍스 셰이더 ──────────────────
VSOut VSMain(VSIn v)
{
    VSOut o;

    //── 스킨 포지션
    float4 skinnedPos = float4(v.posL, 1);
    if (boneCount)
    {
        skinnedPos = 0;
        [unroll]
        for (int i = 0; i < 4; ++i)
        {
            float w = v.boneWeights[i];
            uint idx = v.boneIndices[i];
            skinnedPos += mul(float4(v.posL, 1), gBones[idx]) * w;
        }
    }

    //── 스킨 노멀
    float3 skinnedN = v.normalL;
    if (boneCount)
    {
        skinnedN = 0;
        [loop]
        for (int i = 0; i < 4; ++i)
        {
            float w = v.boneWeights[i];
            uint idx = v.boneIndices[i];
            skinnedN += mul((float3x3) gBones[idx], v.normalL) * w;
        }
    }

    //── 월드 변환
    float4 posW = mul(skinnedPos, world);
    float3 normalW = normalize(mul((float3x3) world, skinnedN));

    //── MVP
    float4 posV = mul(posW, view);
    o.posH = mul(posV, proj);

    //── 출력
    o.posW = posW.xyz;
    o.normalW = normalW;
    o.uv = v.uv;
    return o;
}

//────────────────── 픽셀 셰이더 ──────────────────
float4 PSMain(VSOut input) : SV_TARGET
{
    float4 texColor = useTexture ? gTexture.Sample(gSampler, input.uv) : float4(1, 1, 1, 1);
    float3 N = normalize(input.normalW);
    float3 diffuseSum = float3(0, 0, 0);

    // 여러 라이트 누적
    for (int i = 0; i < gLightCount; ++i)
    {
        float3 L;
        float attenuation = 1.0f;

        if (gLight[i].type == LIGHT_TYPE_DIRECTIONAL)
        {
            L = normalize(-gLight[i].direction);
        }
        else if (gLight[i].type == LIGHT_TYPE_POINT)
        {
            float3 toLight = gLight[i].position - input.posW;
            float dist = length(toLight);
            L = toLight / dist;

            // 선형 감쇠 (range 내에서만 영향)
            attenuation = saturate(1.0f - dist / gLight[i].range);
        }
        else
        {
            // 스폿라이트는 구현하지 않음
            continue;
        }

        float NdotL = saturate(dot(N, L));
        float3 diffuse = gLight[i].color * NdotL * gLight[i].intensity * attenuation;

        diffuseSum += diffuse;
    }

    // 베이스 컬러와 곱하기
    float3 finalColor = texColor.rgb * diffuseSum;

    //return float4(gLightCount / 10.0, 0, 0, 1);
    return float4(saturate(finalColor), texColor.a);
}