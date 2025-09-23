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
    float2 startEndSpeed;
    
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
    
    float4 instance_row0 : INSTANCE0;
    float4 instance_row1 : INSTANCE1;
    float4 instance_row2 : INSTANCE2;
    float4 instance_row3 : INSTANCE3;
    
    float4 randomSeed : INSTANCE4;
    float4 startSizeMin : INSTANCE5;
    float4 startSizeMax : INSTANCE6;
    float4 endSizeMin : INSTANCE7;
    float4 endSizeMax : INSTANCE8;
    float4 startVelocity : INSTANCE9;
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
    
    float randValue = v.randomSeed.x;
    float startSizeValueR = lerp(v.startSizeMin.w, v.startSizeMax.w, randValue);
    float endSizeValueR = lerp(v.endSizeMin.w, v.endSizeMax.w, randValue);
    
    float sizeValueR = lerp(startSizeValueR, endSizeValueR, (time / lifeTime));
    
    float4x4 sizeMat =
    {
        float4(sizeValueR, 0, 0, 0),
        float4(0, sizeValueR, 0, 0),
        float4(0, 0, sizeValueR, 0),
        float4(0, 0, 0, 1)
    };

    // billboard 회전 행렬
    float4x4 billboardRot =
    {
        float4(right, 0),
        float4(up, 0),
        float4(toCamera, 0),
        float4(0, 0, 0, 1)
    };

    float4 posL = float4(v.posL, 1);

    // 크기, billboard 회전 적용
    posL = mul(posL, sizeMat);
    posL = mul(posL, billboardRot);

    // 인스턴스 월드행렬 * 오브젝트 월드행렬
    float4x4 w = mul
    (
        float4x4(v.instance_row0, v.instance_row1, v.instance_row2, v.instance_row3),
        world
    );

    float4 posW = mul(posL, w);
    float speedL = lerp(startEndSpeed.x, startEndSpeed.y, (time / lifeTime));
    posW.xyz += normalize(v.startVelocity.xyz) * (time / lifeTime) * speedL;

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