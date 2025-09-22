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
    float2 padding0;
    float4 startColor;
    float4 endColor;
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

    // 카메라-파티클 방향 벡터 계산
    float3 instancePos = v.instance_row3.xyz;
    float3 toCamera = normalize(pos - instancePos); // 카메라 위치 - 파티클 위치
    float3 up = float3(0, 1, 0);
    float3 right = normalize(cross(up, toCamera));
    up = cross(toCamera, right);

    // billboard 회전 행렬
    float4x4 billboardRot =
    {
        float4(right, 0),
        float4(up, 0),
        float4(toCamera, 0),
        float4(0, 0, 0, 1)
    };

    float4 posL = float4(v.posL, 1);

    // billboard 회전 적용
    posL = mul(posL, billboardRot);

    // 인스턴스 월드행렬 * 오브젝트 월드행렬
    float4x4 w = mul
    (
        float4x4(v.instance_row0, v.instance_row1, v.instance_row2, v.instance_row3),
        world
    );

    float4 posW = mul(posL, w);
    posW.xyz += v.startVelocity.xyz * time;

    float4 posV = mul(posW, view);
    float4 posH = mul(posV, proj);

    o.posH = posH;
    o.posW = posW.xyz;
    o.uv = v.uv;

    return o;
}

// 픽셀 셰이더
float4 PSMain(VSOut input) : SV_TARGET
{
    float4 texColor = useTexture ? gBasemap.Sample(gSampler, input.uv) * baseColor : baseColor;
    
    float4 start = texColor * startColor;
    float4 end = texColor * endColor;
   
    float4 finalColor = lerp(start, end, time / lifeTime);
    
    if (finalColor.a < 0.01f)
        discard;
    
    return float4(finalColor.rgb, finalColor.a);
}