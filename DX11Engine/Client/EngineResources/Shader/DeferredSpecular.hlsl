// DeferredSpecular.hlsl
// 목적: Specular 하이라이트만 출력(나머지 검정)
// 입력: t0 Albedo(미사용, 바인딩 호환), t1 Normal, t2 Depth
//       b4 Light[64], b5 InvViewProj, b6 CameraWorldPos(+smoothness/scale)

#define MAX_LIGHTS 64

#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT       1
#define LIGHT_TYPE_SPOT        2

cbuffer PerObject : register(b0)
{
    row_major float4x4 world; // fullscreen quad용
};

cbuffer PerCamera : register(b1)
{
    float3 camPos_dummy; // fullscreen pass에서는 엔진 호환용(미사용)
    row_major float4x4 view;
    row_major float4x4 proj;
    float cpadding;
};

#pragma pack_matrix(row_major)
cbuffer PerLight : register(b4)
{
    float4x4 gLight[MAX_LIGHTS];
};

#pragma pack_matrix(row_major)
cbuffer PerCustomValue : register(b5)
{
    float4x4 gInvViewProj; // 반드시 "row_major" 규칙으로 업데이트/저장할 것(현재 코드와 일치)
};

// Specular 계산에 필요한 추가 정보(권장)
cbuffer PerSpecularParams : register(b6)
{
    float3 gCamPosW; // 실제 카메라 월드 위치 (중요)
    float gSmoothness; // 0..1 (0=거칠, 1=매끈). 없으면 0이라 스펙 안 나옵니다.

    float gSpecularScale; // 스펙 세기(예: 1~8)
    float3 _padSpec;
};

Texture2D gAlbedo : register(t0); // 미사용(바인딩 호환)
Texture2D gNormal : register(t1);
Texture2D<float> gDepth : register(t2);
SamplerState gSampler : register(s0);

struct VSIn
{
    float3 posL : POSITION;
    float2 uv : TEXCOORD0;
};

struct VSOut
{
    float4 posH : SV_POSITION;
    float2 uv : TEXCOORD0; // 0..1 (mesh uv)
};

VSOut VSMain(VSIn v)
{
    VSOut o;
    float4 posW = mul(float4(v.posL, 1), world);
    float4 posV = mul(posW, view);
    o.posH = mul(posV, proj);
    o.uv = v.uv;
    return o;
}

float3 DecodeNormal(float3 enc01)
{
    return normalize(enc01 * 2.0f - 1.0f);
}

// uvScreen: NDC 변환용(뒤집지 않은 화면 UV)
// depth01: 0..1 (D3D depth)
float3 ReconstructWorldPos(float2 uvScreen, float depth01)
{
    float2 ndc = float2(uvScreen.x * 2.0f - 1.0f,
                        uvScreen.y * 2.0f - 1.0f);

    float4 clip = float4(ndc, depth01, 1.0f);

    float4 w = mul(clip, gInvViewProj);
    return w.xyz / max(w.w, 1e-6f);
}

// smoothness(0..1) -> specPower
float SpecPowerFromSmoothness(float s)
{
    // 너무 낮으면 하이라이트가 넓고 밋밋, 너무 높으면 점처럼 보임
    // 8 ~ 256 범위 권장 (원하면 512까지)
    return lerp(8.0f, 256.0f, saturate(s));
}

float4 PSMain(VSOut i) : SV_Target
{
    return float4(0.f, 0.f, 1.f, 1.f);
    
    // uvScreen: NDC 재구성용(뒤집지 않음)
    float2 uvScreen = i.uv;

    // uvTex: SRV 샘플링용(Y flip)
    float2 uvTex = float2(i.uv.x, 1.0f - i.uv.y);

    float depth01 = gDepth.SampleLevel(gSampler, uvTex, 0);

    // 배경/빈공간: 완전 검정
    if (depth01 >= 0.999999f)
        return float4(0, 0, 0, 1);

    float3 N = DecodeNormal(gNormal.Sample(gSampler, uvTex).xyz);
    float3 posW = ReconstructWorldPos(uvScreen, depth01);

    // View vector
    float3 V = normalize(gCamPosW - posW);

    float3 specSum = 0;

    int lightCount = (int) gLight[0][3][3];

    // 스펙 파라미터
    float specPower = SpecPowerFromSmoothness(gSmoothness);
    float specScale = max(gSpecularScale, 0.0f);

    [loop]
    for (int li = 0; li < lightCount; ++li)
    {
        // enabled
        if (gLight[li][3][2] < 0.5f)
            continue;

        uint lightType = (uint) gLight[li][3][0];
        float3 lightPos = float3(gLight[li][0][0], gLight[li][0][1], gLight[li][0][2]);
        float3 lightDir = float3(gLight[li][1][0], gLight[li][1][1], gLight[li][1][2]);
        float3 lightCol = float3(gLight[li][2][0], gLight[li][2][1], gLight[li][2][2]);

        float intensity = gLight[li][1][3];
        float range = gLight[li][0][3];
        float attenK = gLight[li][3][1];

        float3 L = 0;
        float att = 1.0f;

        if (lightType == LIGHT_TYPE_DIRECTIONAL)
        {
            L = normalize(-lightDir);
            att = 1.0f;
        }
        else if (lightType == LIGHT_TYPE_POINT)
        {
            float3 toL = lightPos - posW;
            float distSq = dot(toL, toL);
            float rangeSq = range * range;

            if (range <= 1e-6f || distSq >= rangeSq)
                continue;

            float dist = sqrt(distSq);
            L = toL / max(dist, 1e-6f);

            // range 컷 부드럽게
            float falloff = saturate(1.0f - dist / range);
            falloff *= falloff;

            // range 정규화 inverse-square (반경이 너무 줄어드는 문제 완화)
            float invSqNorm = rangeSq / max(distSq, 1e-3f);

            // 중심 폭주 방지(과도한 흰점 방지)
            invSqNorm = min(invSqNorm, 16.0f);

            // attenK는 "추가 스케일"로 사용(원하면 1/(1+attenK*distSq)로 바꿔도 됨)
            float phys = 1.0f / (1.0f + attenK * distSq);

            att = falloff * invSqNorm * phys;
        }
        else
        {
            continue;
        }

        // specular: Blinn-Phong
        float ndotl = saturate(dot(N, L));
        if (ndotl <= 0.0f)
            continue;

        float3 H = normalize(L + V);
        float ndoth = saturate(dot(N, H));

        float spec = pow(ndoth, specPower);

        // 최종 스펙 누적(하이라이트만)
        specSum += lightCol * (spec * intensity * att * specScale);
    }

    // 하이라이트만 출력, 나머지는 검정
    // HDR로 쓰려면 saturate 제거하고 RT가 FP16이면 그대로 두는 편이 좋습니다.
    return float4(saturate(specSum), 1);
}
