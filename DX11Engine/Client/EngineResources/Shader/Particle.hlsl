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
}

// 텍스처 & 샘플러
Texture2D gBasemap : register(t0);
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
    float2 uv : TEXCOORD0;
};

// 버텍스 셰이더
VSOut VSMain(VSIn v)
{
    VSOut o;

    // 스킨 포지션
    float4 pos = float4(v.posL, 1);

    // 스킨 노멀
    float3 skinnedN = v.normalL;
    
    float4x4 worldMatrix;
    
    worldMatrix = float4x4
        (
            v.instance_row0,
            v.instance_row1,
            v.instance_row2,
            v.instance_row3
        );
        
    worldMatrix = mul(worldMatrix, world);
    
    // 월드 변환

    float4 posW = mul(pos, worldMatrix);
    float3 normalW = normalize(mul(skinnedN, (float3x3) worldMatrix));
    float3 tangentW = normalize(mul(v.tangentL, (float3x3) worldMatrix));

    // MVP
    float4 posV = mul(posW, view);
    o.posH = mul(posV, proj);

    // 출력
    o.posW = posW.xyz;
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