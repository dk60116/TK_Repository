cbuffer MatrixBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
};

cbuffer MaterialBuffer : register(b1)
{
    float4 diffuseColor;
    bool useTexture;
    float3 _padding;
};

cbuffer LightBuffer : register(b2)
{
    float3 lightDirection;
    float _pad1;
    float3 lightColor;
    float _pad2;
};

Texture2D diffuseMap : register(t0);
SamplerState samp : register(s0);

struct VS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output;

    float4 worldPos = mul(float4(input.pos, 1.f), world);
    float4 viewPos = mul(worldPos, view);
    output.pos = mul(viewPos, proj);

    output.normal = mul(float4(input.normal, 0.f), world).xyz;
    output.uv = input.uv;

    return output;
}

float4 PS_Main(VS_OUT input) : SV_TARGET
{
    float3 N = normalize(input.normal);
    float3 L = normalize(-lightDirection);
    float NdotL = saturate(dot(N, L));

    float3 baseColor;

    if (useTexture)
    {
        baseColor = diffuseMap.Sample(samp, input.uv).rgb;
    }
    else
    {
        baseColor = diffuseColor.rgb;
    }

    float3 finalColor = baseColor * lightColor * NdotL;
    return float4(finalColor, 1.0f);
}