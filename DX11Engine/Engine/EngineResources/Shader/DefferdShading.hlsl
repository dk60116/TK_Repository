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
Texture2D gDepthTexture : register(t3);

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
    float4 c = gNormalTexture.Sample(gSampler, i.uv);
    
    if (c.x == 1.f && c.y == 1.f && c.z == 1.f)
        return float4(1.f, 1.f, 1.f, 1.f);
    
    float3 N = normalize(gNormalTexture.Sample(gSampler, i.uv).xyz * 2.0f - 1.0f);

    // 2) 화면 좌표 -> clip, view, world 로 복원 (포인트 라이트용)
    //    NDC xy는 uv로부터 계산, z는 GBuffer에서, w는 clipW 사용
    float2 ndc;
    ndc.x = i.uv.x * 2.0f - 1.0f;
    ndc.y = (1.0f - i.uv.y) * 2.0f - 1.0f;

    float2 depthRG = gDepthTexture.Sample(gSampler, i.uv).xy; // (ndcZ, clipW)
    float ndcZ = depthRG.x;
    float clipW = max(depthRG.y, 1e-6f);

    float4 posC = float4(ndc.x * clipW, ndc.y * clipW, ndcZ * clipW, clipW);

    //float4x4 invProj = inverse(proj);
    //float4x4 invView = inverse(view);

    float4 posV = mul(posC, proj);
    posV /= max(posV.w, 1e-6f);

    float4 posW = mul(posV, view);
    posW /= max(posW.w, 1e-6f);

    // 3) 조명 누적 (디퓨즈 + 앰비언트)
    float3 diffuseSum = 0;
    float3 ambientSum = 0;

    uint lightCount = (uint) gLight[0][3][3];

    [loop]
    for (uint li = 0; li < lightCount; ++li)
    {
        // light payload
        uint type = (uint) gLight[li][3][0];
        float3 lightPos = float3(gLight[li][0][0], gLight[li][0][1], gLight[li][0][2]);
        float3 lightDir = float3(gLight[li][1][0], gLight[li][1][1], gLight[li][1][2]);
        float3 lightColor = float3(gLight[li][2][0], gLight[li][2][1], gLight[li][2][2]);
        float intensity = gLight[li][1][3];
        float range = gLight[li][0][3];
        float attenK = gLight[li][3][1];
        float ambientK = gLight[li][2][3];

        float3 L;
        float attenuation = 1.0f;

        if (type == LIGHT_TYPE_DIRECTIONAL)
        {
            L = normalize(-lightDir);
        }
        else if (type == LIGHT_TYPE_POINT)
        {
            float3 toLight = lightPos - posW.xyz; // 복원한 월드 pos 사용
            float dist = length(toLight);
            L = toLight / max(dist, 1e-6f);
            attenuation = saturate(1.0f - dist / max(range, 1e-6f)) * attenK;
        }
        else
        {
            continue;
        }

        float NdotL = saturate(dot(N, L));
        diffuseSum += lightColor * NdotL * intensity * attenuation;
        ambientSum += lightColor * ambientK;
    }

    // 살짝 바닥값 줄 거면 아래 활성화
     diffuseSum = max(diffuseSum, float3(0.2f, 0.2f, 0.2f));

    float3 final = saturate(ambientSum + diffuseSum);

    // 그레이스케일로 출력(Combine에서 곱셈)
    float g = saturate(max(final.r, max(final.g, final.b)));
    return float4(g, g, g, 1.0f);
}