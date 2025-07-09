cbuffer PerFrame : register(b0)
{
    matrix View;
    matrix Projection;
};

struct VS_IN
{
    float3 pos : POSITION;
    float4 col : COLOR0;
};

struct VS_OUT
{
    float4 posH : SV_POSITION;
    float4 col : COLOR0;
};

VS_OUT VSMain(VS_IN input)
{
    VS_OUT output;
    float4 worldPos = float4(input.pos, 1);
    output.posH = mul(worldPos, View);
    output.posH = mul(output.posH, Projection);
    output.col = input.col;
    return output;
}

float4 PSMain(VS_OUT input) : SV_TARGET
{
    return input.col;
}
