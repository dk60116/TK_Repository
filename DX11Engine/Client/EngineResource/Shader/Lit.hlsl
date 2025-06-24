//--------------------------------------------------------------------------------------
//  공간: 왼손 LH (DirectX 기본) / 단위: 미터
//  텍스처 샘플은 sRGB → 리니어 변환(pxr automatic)
//--------------------------------------------------------------------------------------
static const float PI = 3.14159265f;

// ──────────────────────── Constant-Buffers
cbuffer PerObject : register(b0) // world 변환
{
    float4x4 gWorld;
}

cbuffer PerCamera : register(b1) // 뷰·투영 + 카메라 위치
{
    float4x4 gView;
    float4x4 gProj;
    float3 gCameraPos;
    float _padding1;
}

cbuffer PerFrame : register(b2) // 주광원
{
    float3 gLightDir; // 방향(정규화, 월드 공간)
    float gLightIntensity;
    float3 gLightColor; // RGB(리니어)
    float _pad2;
    float3 gAmbientSkyColor; // 스카이박스 색
    float _pad3;
}

cbuffer PerMaterial : register(b3) // 머티리얼 파라미터
{
    float4 gBaseColor = float4(1, 1, 1, 1);
    float gMetallic = 0.0;
    float gRoughness = 0.5;
    float2 _pad4;
}

// ──────────────────────── 리소스
Texture2D gAlbedoMap : register(t0);
Texture2D gNormalMap : register(t1);
Texture2D gMetalRoughMap : register(t2); // R=Metallic, G=Roughness
TextureCube gEnvMap : register(t3);

SamplerState gLinearClamp : register(s0);
SamplerState gLinearWrap : register(s1);

// ──────────────────────── 버텍스 레이아웃
struct VSIn
{
    float3 posL : POSITION;
    float2 uv : TEXCOORD0;
    float3 nrmL : NORMAL;
    float3 tanL : TANGENT;
};

struct VSOut
{
    float4 posH : SV_POSITION;
    float3 posW : TEXCOORD0;
    float2 uv : TEXCOORD1;
    float3 nrmW : TEXCOORD2;
    float3 tanW : TEXCOORD3;
};

// ──────────────────────── Vertex Shader
VSOut VSMain(VSIn v)
{
    VSOut o;

    float4 posW = mul(float4(v.posL, 1.0f), gWorld);
    o.posH = mul(mul(posW, gView), gProj);
    o.posW = posW.xyz;
    o.uv = v.uv;

    o.nrmW = normalize(mul(float4(v.nrmL, 0), gWorld).xyz);
    o.tanW = normalize(mul(float4(v.tanL, 0), gWorld).xyz);

    return o;
}

// ──────────────────────── PBR 보조 함수
float DistributionGGX(float NdotH, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float d = (NdotH * NdotH) * (a2 - 1.0) + 1.0;
    return a2 / (PI * d * d + 1e-4);
}

float GeometrySmith(float NdotV, float NdotL, float roughness)
{
    float k = pow(roughness + 1.0, 2.0) / 8.0;
    float gL = NdotL / (NdotL * (1.0 - k) + k);
    float gV = NdotV / (NdotV * (1.0 - k) + k);
    return gL * gV;
}

float3 FresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// ──────────────────────── 환경 샘플 (간단 IBL)
float3 SampleEnv(float3 R, float roughness)
{
    // ── 4-인자 버전 : (mipLevel = 0), width, height, levels
    uint w, h, mipLevels;
    gEnvMap.GetDimensions(0, w, h, mipLevels);

    float mip = roughness * (mipLevels - 1);
    return gEnvMap.SampleLevel(gLinearClamp, R, mip).rgb;
}

// ──────────────────────── Pixel Shader
float4 PSMain(VSOut i) : SV_TARGET
{
    return float4(1, 1, 1, 1); // ← 이 상태로 큐브가 보이면 셰이더·텍스처 문제
    
    // 노멀 재구성 (Tangent Space → World)
    float3 N = normalize(i.nrmW);
    float3 T = normalize(i.tanW);
    float3 B = normalize(cross(N, T));
    float3x3 TBN = float3x3(T, B, N);

    float3 nrmTS = gNormalMap.Sample(gLinearWrap, i.uv).xyz * 2.0 - 1.0;
    N = normalize(mul(nrmTS, TBN));

    // 시선 · 광원
    float3 V = normalize(gCameraPos - i.posW);
    float3 L = normalize(-gLightDir);
    float3 H = normalize(L + V);

    // 머티리얼 파라미터
    float3 albedo = gAlbedoMap.Sample(gLinearWrap, i.uv).rgb * gBaseColor.rgb;
    float metallic = lerp(gMetallic, gMetalRoughMap.Sample(gLinearWrap, i.uv).r, 1.0);
    float rough = saturate(gRoughness + gMetalRoughMap.Sample(gLinearWrap, i.uv).g);

    float3 F0 = lerp(float3(0.04, 0.04, 0.04), albedo, metallic);

    float NdotL = saturate(dot(N, L));
    float NdotV = saturate(dot(N, V));
    float NdotH = saturate(dot(N, H));
    float HdotV = saturate(dot(H, V));

    // BRDF
    float D = DistributionGGX(NdotH, rough);
    float G = GeometrySmith(NdotV, NdotL, rough);
    float3 F = FresnelSchlick(HdotV, F0);

    float3 spec = (D * G * F) / max(4.0 * NdotV * NdotL, 1e-4);
    float3 kS = F;
    float3 kD = (1.0 - kS) * (1.0 - metallic);

    float3 direct = (kD * albedo / PI + spec) * gLightColor * gLightIntensity * NdotL;

    // 환경광
    float3 R = reflect(-V, N);
    float3 env = SampleEnv(R, rough);
    float3 ambient = (kD * albedo + spec) * env * gAmbientSkyColor;

    float3 color = direct + ambient;

    // ACES 톤매핑 + 감마 보정
    color = color / (color + 0.155) * 1.019;
    color = pow(saturate(color), 1.0 / 2.2);

    return float4(color, gBaseColor.a);
}

// ──────────────────────── Technique11
technique11 SimpleLit
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VSMain()));
        SetPixelShader(CompileShader(ps_5_0, PSMain()));
    }
}