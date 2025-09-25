// ───────────── 상수 버퍼
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
    float4 baseColor;
    float3 padding; // 16바이트 정렬
};

// ───────────── 버텍스 입력
struct VSIn
{
    float3 posL : POSITION;
    
    float4 instance_row0 : INSTANCE0;
    float4 instance_row1 : INSTANCE1;
    float4 instance_row2 : INSTANCE2;
    float4 instance_row3 : INSTANCE3;
};

// ───────────── 버텍스 출력
struct VSOut
{
    float4 posH : SV_POSITION;
};

// ───────────── 버텍스 셰이더
VSOut VSMain(VSIn v)
{
    VSOut o;

    // 인스턴싱 월드 행렬 구성

    float4x4 finalWorld = world;

    float4 posW = mul(float4(v.posL, 1.0f), finalWorld);
    float4 posV = mul(posW, gView);
    o.posH = mul(posV, gProj);

    return o;
}

// ───────────── 픽셀 셰이더
float4 PSMain(VSOut input) : SV_TARGET
{
    // 단일 색상 출력
    return baseColor;
}