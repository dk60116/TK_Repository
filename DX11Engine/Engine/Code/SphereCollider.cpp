#include "epch.h"
#include "SphereCollider.h"

CSphereCollider::CSphereCollider()
    : m_sLocal({})
    , m_sWorld({})
{
    m_strName = L"Box Collider";
}

CSphereCollider::~CSphereCollider()
{
}

CSphereCollider* CSphereCollider::Create()
{
	return new CSphereCollider();
}

CComponent* CSphereCollider::Clone() const
{
	CSphereCollider* clone = new CSphereCollider();

	clone->m_sLocal = this->m_sLocal;

	return clone;
}

HRESULT CSphereCollider::Initialize()
{
	Build_WorldSPHERE();

	return S_OK;
}

void CSphereCollider::Update()
{
    Build_WorldSPHERE();
}

void CSphereCollider::LateUpdate()
{
}

void CSphereCollider::Render_Editor()
{
    CCamera* cam = CSceneManager::Get_CrtScene()->Get_EditorCamera();

    if (!cam) 
        return;

    _matrix view = cam->Get_ViewMatrix();
    _matrix proj = cam->Get_ProjectionMatrix();
    _matrix VP = XMMatrixMultiply(view, proj);

    const D3D11_VIEWPORT* vp = CGraphicDevice::GetInstance().Get_CurrentViewport();
    ImDrawList* dl = ImGui::GetBackgroundDrawList();
    if (!vp || !dl)
        return;

    auto WorldToScreen = [&](const vector3& p, ImVec2& out)->_bool
    {
        _vector pw = XMVectorSet(p.x, p.y, p.z, 1.0f);
        _vector clip = XMVector4Transform(pw, VP);
        _float w = XMVectorGetW(clip);
        if (fabsf(w) <= 1e-6f)
            return false;

        _float x = XMVectorGetX(clip) / w;
        _float y = XMVectorGetY(clip) / w;
        _float z = XMVectorGetZ(clip) / w;

        if (x < -1.f || x > 1.f || y < -1.f || y > 1.f || z < 0.f || z > 1.f)
            return false;

        _float sx = (x * 0.5f + 0.5f) * vp->Width + vp->TopLeftX;
        _float sy = (-y * 0.5f + 0.5f) * vp->Height + vp->TopLeftY;
        out = ImVec2(sx, sy);
        return true;
    };

    _matrix invV = XMMatrixInverse(nullptr, view);
    _float4x4 iv;
    XMStoreFloat4x4(&iv, invV);
    vector3 camRight(iv._11, iv._12, iv._13);
    camRight = camRight.normalized();

    const SPHERE& s = m_sWorld;

    ImVec2 c2d, r2d;
    if (!WorldToScreen(s.center, c2d)) 
        return;

    vector3 edge = s.center + camRight * s.radius;
    if (!WorldToScreen(edge, r2d))
        return;

    float screenR = sqrtf((r2d.x - c2d.x) * (r2d.x - c2d.x) + (r2d.y - c2d.y) * (r2d.y - c2d.y));
    if (screenR <= 0.5f)
        return;

    ImU32 emptyColor = IM_COL32(0, 230, 0, 255);
    ImU32 enterColor = IM_COL32(230, 0, 0, 255);

    const ColorValue& pairColorF = CCollisionManager::Get_GizmoColorPair(m_eGizmoColor).first;
    const ColorValue& pairColorS = CCollisionManager::Get_GizmoColorPair(m_eGizmoColor).second;

    emptyColor = IM_COL32(pairColorF.r, pairColorF.g, pairColorF.b, 255);
    enterColor = IM_COL32(pairColorS.r, pairColorS.g, pairColorS.b, 255);

    ImU32 col = m_mEnteredColliders.size() <= 0 ? emptyColor : enterColor;
    dl->AddCircle(c2d, screenR, col, 48, 1.5f);
}

void CSphereCollider::OnDestroy()
{
    __super::OnDestroy();
}

void CSphereCollider::Set_Center(const vector3 _center)
{
	m_sLocal.center = _center;
	Build_WorldSPHERE();
}

void CSphereCollider::Set_Size(const _float _size)
{
	m_sLocal.radius = _size * 0.5f;
    Build_WorldSPHERE();
}

void CSphereCollider::Build_WorldSPHERE()
{
    CTransform* tr = m_pGameObject->Get_Transform();

    _matrix W = tr->Get_WorldMatrix();
    _float4x4 w4x4;
    XMStoreFloat4x4(&w4x4, W);

    // Row 기반 축(당신의 BoxCollider와 동일)
    vector3 Xraw(w4x4._11, w4x4._12, w4x4._13);
    vector3 Yraw(w4x4._21, w4x4._22, w4x4._23);
    vector3 Zraw(w4x4._31, w4x4._32, w4x4._33);
    vector3 T(w4x4._41, w4x4._42, w4x4._43);

    _float sx = Xraw.length();
    _float sy = Yraw.length();
    _float sz = Zraw.length();

    // 월드센터 = T + (R*S) * localCenter
    const vector3 lc = m_sLocal.center;
    vector3 worldC = T + Xraw * lc.x + Yraw * lc.y + Zraw * lc.z;

    // 반지름: 보수적 처리(최대 스케일)
    _float s = max(sx, max(sy, sz));
    _float worldR = fabsf(m_sLocal.radius) * s;

    m_sWorld.center = worldC;
    m_sWorld.radius = worldR;
}

_bool CSphereCollider::IntersectSPHEREToSPHERE(const SPHERE& _sphereA, const SPHERE& _sphereB, _float* _outPen, vector3* _outAxis)
{
    const vector3 d = _sphereB.center - _sphereA.center; // A->B
    const _float rSum = _sphereA.radius + _sphereB.radius;

    const _float distSq = d.lengthSq();
    const _float rSumSq = rSum * rSum;

    if (distSq >= rSumSq)
    {
        if (_outPen)  *_outPen = 0.f;
        if (_outAxis) *_outAxis = vector3::zero();
        return false;
    }

    const _float EPS = 1e-6f;
    const _float dist = sqrtf(max(distSq, EPS));
    const _float penetration = rSum - dist;

    if (_outPen)
        *_outPen = penetration;

    if (_outAxis) 
    {
        if (dist > EPS)
            *_outAxis = d / dist;
        else
            *_outAxis = vector3::up(); 
    }
    
    return true;
}

const CSphereCollider::SPHERE& CSphereCollider::Get_WorldSPHERE()
{
    return m_sWorld;
}
