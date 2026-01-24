// ShadowDepth

cbuffer PerObject : register(b0)
{
    float4x4 world;
};

cbuffer PerCamera : register(b1)
{
    float3 camPos; // unused
    float _pad0;
    float4x4 view; // LightView
    float4x4 proj; // LightProj
};

cbuffer PerMaterial : register(b2)
{
    float4 baseColor; // unused (바인딩 호환)
    uint useTexture; // unused
    uint boneCount;
    uint useNormalMap; // unused
    uint _padMat0;
};

cbuffer PerBones : register(b3)
{
    float4x4 gBones[128];
};

struct VSIn
{
    float3 posL : POSITION;
    uint4 boneIndices : BLENDINDICES;
    float4 boneWeights : BLENDWEIGHT;
};

struct VSOut
{
    float4 posH : SV_POSITION;
};

VSOut VSMain(VSIn v)
{
    VSOut o;

    float4 skinnedPos = float4(v.posL, 1.0f);

    if (boneCount != 0)
    {
        skinnedPos = 0;

        [unroll]
        for (int i = 0; i < 4; ++i)
        {
            float w = v.boneWeights[i];
            uint idx = v.boneIndices[i];

            if (w > 0.0f && idx < 128)
            {
                float4x4 M = gBones[idx];
                skinnedPos += mul(float4(v.posL, 1.0f), M) * w;
            }
        }
    }

    float4 posW = mul(skinnedPos, world);
    float4 posV = mul(posW, view);
    o.posH = mul(posV, proj);
    return o;
}

float4 PSMain(VSOut i) : SV_Target
{
    return 0;
}
