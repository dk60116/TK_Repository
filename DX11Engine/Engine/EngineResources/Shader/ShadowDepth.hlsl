#pragma pack_matrix(row_major)

cbuffer PerObject : register(b0)
{
    row_major float4x4 world;
};

cbuffer PerCamera : register(b1)
{
    float3 camPos; // 미사용(바인딩 호환)
    float pad;
    row_major float4x4 view; // 여기서는 LightView
    row_major float4x4 proj; // 여기서는 LightProj
};

cbuffer PerMaterial : register(b2)
{
    float4 baseColor; // 미사용
    uint useTexture; // 미사용
    uint boneCount;
    uint pad0;
};

cbuffer PerBones : register(b3)
{
    row_major float4x4 gBones[128];
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
        float4 p = 0;

        [unroll]
        for (int i = 0; i < 4; ++i)
        {
            float w = v.boneWeights[i];
            uint idx = v.boneIndices[i];

            if (w > 0.0f && idx < 128)
            {
                p += mul(float4(v.posL, 1.0f), gBones[idx]) * w;
            }
        }

        skinnedPos = p;
    }

    float4 posW = mul(skinnedPos, world);
    float4 posV = mul(posW, view);
    o.posH = mul(posV, proj);

    return o;
}

float PSMain(VSOut i) : SV_Depth
{
    return i.posH.z / i.posH.w;
}
