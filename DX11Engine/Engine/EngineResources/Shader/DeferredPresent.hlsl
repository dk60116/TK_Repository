cbuffer PerObject : register(b0)
{
    float4x4 world;
};

cbuffer PerCamera : register(b1)
{
    float3 pos;
    float _pad0;
    float4x4 view;
    float4x4 proj;
};

Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

struct VSIn
{
    float3 posL : POSITION; // VertexTexNormalTangentBuffer.position
    float2 uv : TEXCOORD0; // VertexTexNormalTangentBuffer.uv
};

struct VSOut
{
    float4 posH : SV_POSITION;
    float2 uv : TEXCOORD0;
};

VSOut VSMain(VSIn v)
{
    VSOut o;
    float4 posW = mul(float4(v.posL, 1.0f), world);
    float4 posV = mul(posW, view);
    o.posH = mul(posV, proj);
    o.uv = v.uv;
    return o;
}

float4 PSMain(VSOut input) : SV_Target
{
    // 1) "무조건 보이기" 확인용
    return float4(1.f, 1.f, 1.f, 1.f);

    // 2) 텍스처 출력으로 바꾸려면 아래로 교체
    // return gTexture.Sample(gSampler, input.uv);
}
