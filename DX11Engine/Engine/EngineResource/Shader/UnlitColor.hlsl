cbuffer PerObject : register(b0)
{
    float4x4 gWorldViewProj;
    float4 gBaseColor;
}

cbuffer PerCamera : register(b1)
{
    float4x4 gView;
    float4x4 gProj;
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
    o.posH = mul(float4(v.posL, 1), gWorldViewProj);
    return o;
}

// ───── Pixel Shader
float4 PSMain(VSOut input) : SV_TARGET
{
    return gBaseColor;
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
