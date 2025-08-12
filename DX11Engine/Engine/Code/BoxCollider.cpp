#include "epch.h"
#include "BoxCollider.h"

CBoxCollider::CBoxCollider()
    : CCollider()
	, m_sLocal({})
	, m_sWorld({})
{
}

CBoxCollider::~CBoxCollider()
{
}

CBoxCollider* CBoxCollider::Create()
{
	return new CBoxCollider();
}

CBoxCollider* CBoxCollider::Clone() const
{
	CBoxCollider* clone = new CBoxCollider();

	return clone;
}

HRESULT CBoxCollider::Initialize()
{
    m_sLocal.center = vector3::zero();
    m_sLocal.halfExtents = vector3(0.5f, 0.5f, 0.5f);
    m_sLocal.axis[0] = vector3::right();
    m_sLocal.axis[1] = vector3::up();
    m_sLocal.axis[2] = vector3::forward();

    CDebug::LogError("InitBox");

	return S_OK;
}

void CBoxCollider::Update()
{
    Build_WorldOBB();
}

void CBoxCollider::LateUpdate()
{
}

void CBoxCollider::Render()
{
}

void CBoxCollider::Render_Editor()
{
    CCamera* editorCam = CSceneManager::GetInstance().Get_CrtScene()->Get_EditorCamera();

    if (!editorCam)
        return;

    _matrix view = editorCam->Get_ViewMatrix();
    _matrix proj = editorCam->Get_ProjectionMatrix();
    _matrix VP = XMMatrixMultiply(view, proj);

    const D3D11_VIEWPORT* vp = CGraphicDevice::GetInstance().Get_CurrentViewport();
    ImDrawList* dl = ImGui::GetBackgroundDrawList();
    
    if (!vp || !dl) 
        return;

    const OBB& b = m_sWorld;
    const vector3 ex = b.axis[0] * b.halfExtents.x;
    const vector3 ey = b.axis[1] * b.halfExtents.y;
    const vector3 ez = b.axis[2] * b.halfExtents.z;

    const vector3 c000 = b.center - ex - ey - ez;
    const vector3 c100 = b.center + ex - ey - ez;
    const vector3 c010 = b.center - ex + ey - ez;
    const vector3 c110 = b.center + ex + ey - ez;
    const vector3 c001 = b.center - ex - ey + ez;
    const vector3 c101 = b.center + ex - ey + ez;
    const vector3 c011 = b.center - ex + ey + ez;
    const vector3 c111 = b.center + ex + ey + ez;

    auto WorldToScreen = [&](const vector3& p, ImVec2& out)->_bool 
        {
        _vector pw = XMVectorSet(p.x, p.y, p.z, 1.0f);
        _vector clip = XMVector4Transform(pw, VP);
        _float w = XMVectorGetW(clip);

        if (w <= 1e-6f)
            return false;

        _float ndcX = XMVectorGetX(clip) / w;
        _float ndcY = XMVectorGetY(clip) / w;

        _float sx = (ndcX * 0.5f + 0.5f) * vp->Width + vp->TopLeftX;
        _float sy = (-ndcY * 0.5f + 0.5f) * vp->Height + vp->TopLeftY;
        out = ImVec2(sx, sy);
        return true;
        };

    auto DrawEdge = [&](const vector3& a, const vector3& b)
        {
        ImVec2 sa, sb;
        if (WorldToScreen(a, sa) && WorldToScreen(b, sb))
            dl->AddLine(sa, sb, m_mEnteredColliders.size() <= 0 ? IM_COL32(0, 230, 0, 255) : IM_COL32(230, 0, 0, 255), 1.5f);
        };

    DrawEdge(c000, c100);
    DrawEdge(c100, c110);
    DrawEdge(c110, c010);
    DrawEdge(c010, c000);

    DrawEdge(c001, c101);
    DrawEdge(c101, c111);
    DrawEdge(c111, c011);
    DrawEdge(c011, c001);

    DrawEdge(c000, c001);
    DrawEdge(c100, c101);
    DrawEdge(c010, c011);
    DrawEdge(c110, c111);
}

void CBoxCollider::OnDestroy()
{
}

void CBoxCollider::Set_Center(vector3 _center)
{
	m_sLocal.center = _center;
	Build_WorldOBB();
}

void CBoxCollider::Set_Size(vector3 _size)
{
	m_sLocal.halfExtents = _size * 0.5f;
    Build_WorldOBB();
}

void CBoxCollider::Build_WorldOBB()
{
    CTransform* tr = m_pGameObject->Get_Transform();

    _matrix W = tr->Get_WorldMatrix();
    _float4x4 w4x4;

	XMStoreFloat4x4(&w4x4, W);

    vector3 Xraw(w4x4._11, w4x4._12, w4x4._13);
    vector3 Yraw(w4x4._21, w4x4._22, w4x4._23);
    vector3 Zraw(w4x4._31, w4x4._32, w4x4._33);
    vector3 T   (w4x4._41, w4x4._42, w4x4._43);

    _float sx = Xraw.length(); 
    vector3 axX = (sx > 1e-8f) ? (Xraw / sx) : vector3::right();
    _float sy = Yraw.length();
    vector3 axY = (sy > 1e-8f) ? (Yraw / sy) : vector3::up();
    _float sz = Zraw.length();
    vector3 axZ = (sz > 1e-8f) ? (Zraw / sz) : vector3::forward();

    const vector3 lc = m_sLocal.center;
    vector3 worldC = T + Xraw * lc.x + Yraw * lc.y + Zraw * lc.z;

    vector3 he = m_sLocal.halfExtents;
    vector3 worldHE(he.x * sx, he.y * sy, he.z * sz);

    m_sWorld.center = worldC;
    m_sWorld.axis[0] = axX;
    m_sWorld.axis[1] = axY;
    m_sWorld.axis[2] = axZ;
    m_sWorld.halfExtents = worldHE;
}

_bool CBoxCollider::Intersect(const OBB& _boxA, const OBB& _boxB, _float* _outPen, vector3* _outAxis)
{
    const _float EPS = 1e-6f;
    const vector3 T = _boxB.center - _boxA.center; // A->B

    const vector3 Aax[3] = { _boxA.axis[0], _boxA.axis[1], _boxA.axis[2] };
    const vector3 Bax[3] = { _boxB.axis[0], _boxB.axis[1], _boxB.axis[2] };

    _float minOverlap = FLT_MAX;
    vector3 minAxis = vector3::zero();

    auto TEST_AXIS = [&](const vector3& axis)->bool 
    {
        if (axis.lengthSq() < EPS) 
            return true; 

        const vector3 n = axis.normalized();
        const _float rA = ProjectionRadius(_boxA, n);
        const _float rB = ProjectionRadius(_boxB, n);
        const _float dist = fabsf(vector3::dot(T, n));
        const _float overlap = (rA + rB) - dist;
        if (overlap < 0.f)
            return false;

        if (overlap < minOverlap)
        {
            minOverlap = overlap;
            minAxis = (vector3::dot(T, n) < 0.f) ? (-n) : n;
        }

        return true;
    };

    // 3 + 3 + 9 = 15 รเ
    for (_int i = 0; i < 3; ++i)
    {
        if (!TEST_AXIS(Aax[i]))
            return false;
    }
    for (_int i = 0; i < 3; ++i)
    {
        if (!TEST_AXIS(Bax[i]))
            return false;
    }
    for (_int i = 0; i < 3; ++i)
    {
        for (_int j = 0; j < 3; ++j)
        {
            if (!TEST_AXIS(vector3::Cross(Aax[i], Bax[j]))) 
                return false;
        }
    }

    if (_outPen)
        *_outPen = minOverlap;
    if (_outAxis) 
        *_outAxis = minAxis;
    
    return true;
}

_float CBoxCollider::ProjectionRadius(const OBB& _obb, const vector3& _n)
{
    const _float e0 = _obb.halfExtents.x;
    const _float e1 = _obb.halfExtents.y;
    const _float e2 = _obb.halfExtents.z;

    return fabsf(vector3::dot(_obb.axis[0], _n)) * e0
        + fabsf(vector3::dot(_obb.axis[1], _n)) * e1
        + fabsf(vector3::dot(_obb.axis[2], _n)) * e2;
}

const CBoxCollider::OBB& CBoxCollider::Get_WorldOBB()
{
	return m_sWorld;
}
