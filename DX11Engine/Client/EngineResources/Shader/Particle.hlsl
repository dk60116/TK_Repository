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
    float gCalcLight;
    float2 gTiling;
    float2 gOffset;
    float cPadding;
}

// 라이트 정의
#define MAX_LIGHTS 256

#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

#pragma pack_matrix(row_major)
cbuffer PerLight : register(b4)
{
    float4x4 gLight[MAX_LIGHTS];
};

// 텍스처 & 샘플러
Texture2D gBasemap : register(t0);
Texture2D gNormalmap : register(t1);
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
    float3 posW : TEXCOORD1;
    float3 normalW : TEXCOORD2;
    float3 tangentW : TEXCOORD3;
    float2 uv : TEXCOORD0;
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
    float3 normalW = normalize(mul(skinnedN, (float3x3) worldMatrix));
    float3 tangentW = normalize(mul(v.tangentL, (float3x3) worldMatrix));

    // MVP
    float4 posV = mul(posW, view);
    o.posH = mul(posV, proj);

    // 출력
    o.posW = posW.xyz;
    o.normalW = normalW;
    o.tangentW = tangentW;
    o.uv = v.uv;
    
    return o;
}

// 픽셀 셰이더
float4 PSMain(VSOut input) : SV_TARGET
{
    float4 texColor = useTexture ? gBasemap.Sample(gSampler, input.uv) * baseColor : baseColor;
    
    if (texColor.a < 0.01f)
        discard;

    return float4(texColor.rgb, texColor.a);
}