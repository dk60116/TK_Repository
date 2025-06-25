cbuffer PerObject : register(b0)
{
    float4x4 world;
}

cbuffer PerCamera : register(b1)
{
    float4x4 view;
    float4x4 proj;
}

cbuffer PerMaterial : register(b2)
{
    float4 baseColor;
    bool useTexture;
    float3 padding; // 16바이트 정렬 맞추기 위해 필요
}

// ───── 버텍스 구조
struct VSIn
{
    float3 posL : POSITION;
};

struct VSOut
{
    float4 posH : SV_POSITION;
};

// ───── Vertex Shader
VSOut VSMain(VSIn v)
{
    VSOut o;
    float4 worldPos = mul(float4(v.posL, 1.0f), world);
    float4 viewPos = mul(worldPos, view);
    o.posH = mul(viewPos, proj);
    return o;
}

// ───── Pixel Shader
float4 PSMain(VSOut input) : SV_TARGET
{
    return baseColor;
}

// ───── Technique
technique11 UnlitColor
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VSMain()));
        SetPixelShader(CompileShader(ps_5_0, PSMain()));
    }
}
