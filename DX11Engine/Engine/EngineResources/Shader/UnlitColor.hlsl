// 상수 버퍼
cbuffer PerObject : register(b0)
{
    float4x4 world;
};

cbuffer PerCamera : register(b1)
{
    float3 camPos;
    float4x4 view;
    float4x4 proj;
    float cpadding;
};

cbuffer PerMaterial : register(b2)
{
    float4 baseColor; // rgba 0~1
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
}

//// 라이트 정의
//#define MAX_LIGHTS 64

//#define LIGHT_TYPE_DIRECTIONAL 0
//#define LIGHT_TYPE_POINT 1
//#define LIGHT_TYPE_SPOT 2

//#pragma pack_matrix(row_major)
//cbuffer PerLight : register(b4)
//{
//    float4x4 gLight[MAX_LIGHTS];
//};

// 텍스처 & 샘플러
Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

// 버텍스 입출력
struct VSIn
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float2 uv : TEXCOORD0;
    float3 tangentL : TANGENT;
    uint4 boneIndices : BLENDINDICES;
    float4 boneWeights : BLENDWEIGHT;
    
    float4 instance_row0 : INSTANCE0;
    float4 instance_row1 : INSTANCE1;
    float4 instance_row2 : INSTANCE2;
    float4 instance_row3 : INSTANCE3;
};

struct VSOut
{
    float4 posH : SV_POSITION;
    float3 normalW : NORMAL;
    float3 posW : TEXCOORD1;
    float2 uv : TEXCOORD0;
};

struct PSOut
{
    float4 diffuse : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 depth : SV_TARGET2;
};

// 버텍스 셰이더
VSOut VSMain(VSIn v)
{
    VSOut o;

    // 스킨 포지션
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

    // 스킨 노멀
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
    
    float4x4 worldMatrix;
    
    if (!boneCount)
    {
        worldMatrix = float4x4
        (
            v.instance_row0,
            v.instance_row1,
            v.instance_row2,
            v.instance_row3
        );
        
        worldMatrix = mul(worldMatrix, world);
    }
    else
    {
        worldMatrix = world;
    }
    
    // 월드 변환

    float4 posW = mul(skinnedPos, worldMatrix);
    float4 normalW = float4(1.f, 1.f, 1.f, 1.f);

    // MVP
    float4 posV = mul(posW, view);
    o.posH = mul(posV, proj);

    // 출력
    o.posW = posW.xyz;
    o.normalW = normalW;
    o.uv = v.uv;
    
    return o;
}

// 픽셀 셰이더
PSOut PSMain(VSOut input) : SV_TARGET
{
    PSOut Out = (PSOut) 0;
    
    vector vMtrlDiffuse;
    
    if (useTexture != 0)
        vMtrlDiffuse = gTexture.Sample(gSampler, input.uv) * baseColor;
    else
        vMtrlDiffuse = baseColor;
    
    float ndcZ = input.posH.z * 2.0f - 1.0f;
    float clipW = input.posH.w;
    
    if (vMtrlDiffuse.a < 0.1f)
        discard;
    
    Out.diffuse = vMtrlDiffuse;
    Out.normal = vector(input.normalW.xyz * 0.5f + 0.5f, 1.f);
    Out.depth = vector(ndcZ, clipW, 0.f, 1.f);
    
    return Out;
}