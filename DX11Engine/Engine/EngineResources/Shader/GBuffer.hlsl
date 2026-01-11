// G-Buffer output shader
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
    float4 baseColor;
    uint useTexture;
    uint boneCount;
    float2 mpadding;
};

cbuffer PerBones : register(b3)
{
    float4x4 gBones[128];
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

    float4 i0 : INSTANCE0;
    float4 i1 : INSTANCE1;
    float4 i2 : INSTANCE2;
    float4 i3 : INSTANCE3;
};

struct VSOut
{
    float4 posH : SV_POSITION;
    float3 normalW : NORMAL;
    float3 posW : TEXCOORD1;
    float2 uv : TEXCOORD0;
};

VSOut VSMain(VSIn v)
{
    VSOut o;

    float4x4 iWorld = world;

    if (!(all(v.i0 == 0) && all(v.i1 == 0) && all(v.i2 == 0) && all(v.i3 == 0)))
        iWorld *= float4x4(v.i0, v.i1, v.i2, v.i3);

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

    float3 skinnedN = v.normalL;
    if (boneCount)
    {
        skinnedN = 0;
        [loop]
        for (int i = 0; i < 4; ++i)
        {
            float w = v.boneWeights[i];
            uint idx = v.boneIndices[i];
            skinnedN += mul((float3x3)gBones[idx], v.normalL) * w;
        }
    }

    float4 posW = mul(skinnedPos, iWorld);
    float3 normalW = normalize(mul((float3x3)iWorld, boneCount ? skinnedN : v.normalL));

    float4 posV = mul(posW, view);
    o.posH = mul(posV, proj);

    o.posW = posW.xyz;
    o.normalW = normalW;
    o.uv = v.uv;

    return o;
}

struct PSOut
{
    float4 albedo : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 position : SV_TARGET2;
};

PSOut PSMain(VSOut input)
{
    PSOut o;

    float4 texColor = useTexture ? gTexture.Sample(gSampler, input.uv) : float4(1, 1, 1, 1);
    float4 albedo = texColor * baseColor;

    float3 normalW = normalize(input.normalW);
    o.albedo = albedo;
    o.normal = float4(normalW * 0.5f + 0.5f, 1.0f);
    o.position = float4(input.posW, 1.0f);

    return o;
}
