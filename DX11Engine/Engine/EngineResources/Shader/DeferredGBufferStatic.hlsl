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
    uint useTexture;
    uint boneCount;
    float2 mpadding;
};

cbuffer PerCustomValue : register(b10)
{
    float gSmoothness;
    float gCalcLight;
    float2 gTiling;
    float2 gOffset;
    float cPadding;
};

Texture2D gBasemap : register(t0);
Texture2D gNormalmap : register(t1);
SamplerState gSampler : register(s0);

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
};

struct VSOut
{
    float4 posH : SV_POSITION;
    float3 posW : TEXCOORD1;
    float3 normalW : TEXCOORD2;
    float3 tangentW : TEXCOORD3;
    float2 uv : TEXCOORD0;
};

VSOut VSMain(VSIn v)
{
    VSOut o;

    float4x4 worldMatrix = float4x4
    (
        v.instance_row0,
        v.instance_row1,
        v.instance_row2,
        v.instance_row3
    );

    worldMatrix = mul(worldMatrix, world);

    float4 posW = mul(float4(v.posL, 1.0f), worldMatrix);
    float3 normalW = normalize(mul(v.normalL, (float3x3) worldMatrix));
    float3 tangentW = normalize(mul(v.tangentL, (float3x3) worldMatrix));

    float4 posV = mul(posW, gView);
    o.posH = mul(posV, gProj);

    o.posW = posW.xyz;
    o.normalW = normalW;
    o.tangentW = tangentW;
    o.uv = v.uv;

    return o;
}

struct PSOut
{
    float4 rt0 : SV_Target0;
    float4 rt1 : SV_Target1;
    float4 rt2 : SV_Target2;
};

PSOut PSMain(VSOut input)
{
    PSOut outp;

    float2 tillingUV = float2(input.uv.x * gTiling.x + gOffset.x, input.uv.y * gTiling.y + gOffset.y);
    float4 texColor = useTexture ? gBasemap.Sample(gSampler, tillingUV) * baseColor : baseColor;

    if (texColor.a < 0.01f)
        discard;

    float3 N = normalize(input.normalW);
    float3 T = normalize(input.tangentW);
    float3 B = normalize(cross(N, T));

    float4 nm = gNormalmap.Sample(gSampler, tillingUV);
    if (any(nm))
    {
        float3 normalMap = nm.rgb;
        normalMap = normalize(normalMap * 2.0f - 1.0f);
        float3x3 TBN = float3x3(T, B, N);
        N = normalize(mul(normalMap, TBN));
    }

    outp.rt0 = float4(texColor.rgb, texColor.a);
    outp.rt1 = float4(N * 0.5f + 0.5f, gSmoothness);
    outp.rt2 = float4(input.posW, 1.0f);

    return outp;
}
