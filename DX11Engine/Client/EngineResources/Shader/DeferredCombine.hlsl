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

Texture2D gAlbedo : register(t0);
Texture2D gShading : register(t1);
Texture2D gSpecular : register(t2);
SamplerState gSampler : register(s0);

struct VSIn
{
    float3 posL : POSITION; 
    float2 uv : TEXCOORD0;
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
    float2 uv = input.uv;
    uv.y = 1.0f - uv.y;

    float4 a = gAlbedo.Sample(gSampler, uv);
    float4 s = gShading.Sample(gSampler, uv);
    float4 sp = gSpecular.Sample(gSampler, uv);
    
    float4 as = a * s;
    
    float4 final = as + sp;
    
    return final;
}
