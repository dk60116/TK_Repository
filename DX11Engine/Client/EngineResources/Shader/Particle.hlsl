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
    float campadding;
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
    float time;
    float lifeTime;
    float startDelay;
    float  _pad0;  
    float4 endColor;
    float4 startColor;
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
    
    float4 startVelocity : INSTANCE4;
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

    float4 posL = float4(v.posL, 1);

    // 인스턴스 월드행렬 * 오브젝트 월드행렬
    float4x4 w = mul
    (
        float4x4(v.instance_row0, v.instance_row1, v.instance_row2, v.instance_row3),
        world
    );

    // 월드 위치
    float4 posW = mul(posL, w);

    posW.xyz += v.startVelocity.xyz * time;

    // 뷰/프로젝션
    float4 posV = mul(posW, view);
    float4 posH = mul(posV, proj);

    // 출력
    o.posH = posH;
    o.posW = posW.xyz;
    o.uv = v.uv;
    return o;
}

// 픽셀 셰이더
float4 PSMain(VSOut input) : SV_TARGET
{
    float4 texColor = useTexture ? gBasemap.Sample(gSampler, input.uv) * baseColor : baseColor;
    
    float4 finalColor = lerp(texColor * startColor, texColor * endColor, time / lifeTime);
    
    finalColor = startColor;
    
    if (finalColor.a < 0.01f)
        discard;
    
    return float4(finalColor.rgb, finalColor.a);
}