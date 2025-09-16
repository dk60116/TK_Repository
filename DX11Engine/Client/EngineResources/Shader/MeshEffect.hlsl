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

cbuffer PerCustomValue : register(b10)
{
    float gTime;
    float gDistortionAmount;
    float4 ctpadding;
    float4 ctpadding1;
}

// 텍스처 & 샘플러
Texture2D gTexture : register(t0);
Texture2D gNoiseTexture : register(t3);
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

// 버텍스 셰이더
VSOut VSMain(VSIn v)
{
    VSOut o;

    // Instance * world
    float4x4 worldMatrix = float4x4
    (
        v.instance_row0,
        v.instance_row1,
        v.instance_row2,
        v.instance_row3
    );
    worldMatrix = mul(worldMatrix, world);

    // Transform
    float4 posW = mul(float4(v.posL, 1.0f), worldMatrix);
    float3 normalW = normalize(mul((float3x3) worldMatrix, v.normalL));

    // MVP
    float4 posV = mul(posW, view);
    o.posH = mul(posV, proj);

    // Out
    o.posW = posW.xyz;
    o.normalW = normalW;
    o.uv = v.uv;
    return o;
}


// 픽셀 셰이더
float4 PSMain(VSOut input) : SV_TARGET
{
    float2 noiseUV = input.uv + gTime * 0.1f;

    float2 noiseVec = gNoiseTexture.Sample(gSampler, noiseUV).rg;
    noiseVec = noiseVec * 2.0f - 1.0f;

    float2 distortedUV = input.uv + noiseVec * gDistortionAmount * 0.2f;

    float4 texColor = useTexture ? gTexture.Sample(gSampler, distortedUV) : float4(1, 1, 1, 1);

    texColor *= baseColor;

    return texColor;
}