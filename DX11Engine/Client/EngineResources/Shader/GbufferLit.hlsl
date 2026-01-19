// GBufferLit.hlsl (원본 Lit.hlsl 기반 최소 변경 버전)

cbuffer PerObject : register(b0)
{
    float4x4 world;
};

cbuffer PerCamera : register(b1)
{
    float3 pos;
    float4x4 view;
    float4x4 proj;
    float cpadding; // 원본과 동일 유지
};

cbuffer PerMaterial : register(b2)
{
    float4 baseColor;
    uint useTexture;
    uint boneCount;
    float2 mpadding;
};

cbuffer PerBones : register(b3)
{
    float4x4 gBones[128];
};

cbuffer PerCustomValue : register(b10)
{
    float gSmoothness;
    float2 gTiling;
    float2 gOffset;
}; 

Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

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
    float3 normalW : TEXCOORD0;
    float3 posW : TEXCOORD1;
    float2 uv : TEXCOORD2;
};

VSOut VSMain(VSIn v)
{
    VSOut o;

    float4 skinnedPos = float4(v.posL, 1);
    float3 skinnedN = v.normalL;

    if (boneCount)
    {
        skinnedPos = 0;
        skinnedN = 0;

        [unroll]
        for (int i = 0; i < 4; ++i)
        {
            float w = v.boneWeights[i];
            uint idx = v.boneIndices[i];

            // 안전 가드
            if (w > 0.0f && idx < 128)
            {
                skinnedPos += mul(float4(v.posL, 1), gBones[idx]) * w;
                skinnedN += mul((float3x3) gBones[idx], v.normalL) * w;
            }
        }
    }

    float4 posW = mul(skinnedPos, world);
    float3 normalW = normalize(mul((float3x3) world, skinnedN));

    float4 posV = mul(posW, view);
    o.posH = mul(posV, proj);

    o.posW = posW.xyz;
    o.normalW = normalW;
    o.uv = v.uv;
    
    return o;
}

struct PSOut
{
    float4 Albedo : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Material : SV_Target2;
};

PSOut PSMain(VSOut input)
{
    PSOut o;

    float2 uv = float2(input.uv.x * gTiling.x + gOffset.x,
                       input.uv.y * gTiling.y + gOffset.y);

    float4 texColor = useTexture ? gTexture.Sample(gSampler, uv) : float4(1, 1, 1, 1);

    // Albedo
    float4 albedo = baseColor * texColor;
    o.Albedo = saturate(albedo);

    // Normal (world -> 0~1 encode)
    float3 N = normalize(input.normalW);
    o.Normal = float4(N * 0.5f + 0.5f, 1.0f);

    // Material (Smoothness)
    o.Material = float4(saturate(gSmoothness), 0, 0, 1);
    return o;
}
