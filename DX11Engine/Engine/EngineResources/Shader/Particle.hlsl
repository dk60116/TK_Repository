// 상수 버퍼
cbuffer PerObject : register(b0)
{
    float4x4 world;
};

cbuffer PerCamera : register(b1)
{
    float3 gPos;
    float padding1;

    float4x4 gView;
    float4x4 gProj;
    float4x4 gViewInv;
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

    // 1) 카메라 축 추출( view^-1 사용 )
    float4x4 viewInv = gViewInv;
    float3 camRight = normalize(viewInv[0].xyz);
    float3 camUp = normalize(viewInv[1].xyz);
    float3 camPos = viewInv[3].xyz; // 또는 cbuffer의 pos 사용 가능

    // 2) 인스턴스 기준 정보 (로컬)
    float3 instancePos = v.instance_row3.xyz;
    float3 centerW = mul(float4(instancePos, 1), world).xyz;

    // 3) toCamera (안전장치 포함)
    float3 toCamera = camPos - centerW;
    float lenTC = max(length(toCamera), 1e-6);
    toCamera /= lenTC;

    // (camRight, camUp이 이미 정규직교에 가깝지만, 혹시 위해 재직교)
    camRight = normalize(camRight - camUp * dot(camRight, camUp));
    camUp = normalize(cross(toCamera, camRight)); // 화면 위쪽이 자연스럽도록

    // 4) 랜덤/시간 기반 크기 보간
    float randValue = v.randomSeed.x;
    float startSizeValueR = lerp(v.startSizeMin.w, v.startSizeMax.w, randValue);
    float endSizeValueR = lerp(v.endSizeMin.w, v.endSizeMax.w, randValue);
    float t01 = saturate(time / max(lifeTime, 1e-6));
    float sizeValueR = lerp(startSizeValueR, endSizeValueR, t01);

    // 5) 로컬 쿼드(보통 -0.5~0.5) 를 카메라 축으로 전개 (회전행렬 곱 대신 선형결합)
    float2 l = v.posL.xy * sizeValueR;
    float3 billboardOffset = camRight * l.x + camUp * l.y;

    // 6) 위치 = 인스턴스 위치 + 빌보드 오프셋 + 속도 이동
    float speedL = lerp(startEndSpeed.x, startEndSpeed.y, t01);
    float3 vel = (lenTC > 1e-6) ? normalize(v.startVelocity.xyz) : v.startVelocity.xyz;
    
    float3 posW3 = centerW + billboardOffset + vel * t01 * speedL;
    float4 posW = float4(posW3, 1);

    // 7) 뷰/프로젝션
    float4 posV = mul(posW, gView);
    float4 posH = mul(posV, gProj);

    o.posH = posH;
    o.posW = posW.xyz;
    o.uv = v.uv;

    return o;
}

// 픽셀 셰이더
float4 PSMain(VSOut input) : SV_TARGET
{
    if (time == 0.f)
        discard;
    
    float4 texColor = useTexture ? gBasemap.Sample(gSampler, input.uv) * baseColor : baseColor;
    
    float4 start = texColor * startColor;
    float4 end = texColor * endColor;
   
    float4 finalColor = lerp(start, end, time / lifeTime);
    
    if (finalColor.a < 0.01f)
        discard;
    
    return float4(finalColor.rgb, finalColor.a);
}