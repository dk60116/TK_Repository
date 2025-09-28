#include "epch.h"
#include "BoxCollider.h"

CBoxCollider::CBoxCollider()
    : CCollider()
	, m_sLocal({})
	, m_sWorld({})
{
    m_strName = L"Box Collider";
}

CBoxCollider::~CBoxCollider()
{
}

CBoxCollider* CBoxCollider::Create()
{
	return new CBoxCollider();
}

CComponent* CBoxCollider::Clone() const
{
	CBoxCollider* clone = new CBoxCollider();

    clone->m_sLocal = this->m_sLocal;

	return clone;
}

HRESULT CBoxCollider::Initialize(void* _desc)
{
    if (FAILED(__super::Initialize(_desc)))
        return E_FAIL;

    m_sLocal.axis[0] = vector3::right();
    m_sLocal.axis[1] = vector3::up();
    m_sLocal.axis[2] = vector3::forward();

    Build_WorldOBB();

	return S_OK;
}

void CBoxCollider::Update()
{
    Build_WorldOBB();
}

void CBoxCollider::Render_Editor()
{
    CCamera* editorCam = CSceneManager::Get_CrtScene()->Get_EditorCamera();

    if (!editorCam)
        return;

    if (m_eGizmoColor == GizmoColor::Transparent)
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
        {
            ImU32 emptyColor = IM_COL32(0, 230, 0, 255);
            ImU32 enterColor = IM_COL32(230, 0, 0, 255);

            const ColorValue& pairColorF = CCollisionManager::Get_GizmoColorPair(m_eGizmoColor).first;
            const ColorValue& pairColorS = CCollisionManager::Get_GizmoColorPair(m_eGizmoColor).second;

            emptyColor = IM_COL32(pairColorF.r, pairColorF.g, pairColorF.b, 255);
            enterColor = IM_COL32(pairColorS.r, pairColorS.g, pairColorS.b, 255);

            dl->AddLine(sa, sb, m_mEnteredColliders.size() <= 0 ? emptyColor : enterColor, 1.5f);
        }
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
    __super::OnDestroy();
}

void CBoxCollider::Set_Center(const vector3 _center)
{
	m_sLocal.center = _center;
	Build_WorldOBB();
}

void CBoxCollider::Set_Size(const vector3 _size)
{
	m_sLocal.halfExtents = _size * 0.5f;
    Build_WorldOBB();
}

const CPhysics::RAYCASTHIT CBoxCollider::Raycast(CPhysics::Ray _ray) const
{
    CPhysics::RAYCASTHIT hit = {};

    const OBB& b = m_sWorld;
    const _float EPS = 1e-6f;

    _float dirLen = _ray.dir.length();

    if (dirLen < EPS)
        return hit;

    vector3 d = _ray.dir / dirLen;
    _float maxT = _ray.maxDist / dirLen;

    vector3 p = _ray.origin - b.center;
    _float px = vector3::dot(p, b.axis[0]);
    _float py = vector3::dot(p, b.axis[1]);
    _float pz = vector3::dot(p, b.axis[2]);
    _float dx = vector3::dot(d, b.axis[0]);
    _float dy = vector3::dot(d, b.axis[1]);
    _float dz = vector3::dot(d, b.axis[2]);

    _float tmin = 0.f;
    _float tmax = maxT;

    auto slab = [&](_float p, _float d, _float he)->_bool 
        {
        if (fabsf(d) < EPS) 
        {
            return (p >= -he && p <= he);
        }
        _float t1 = (-he - p) / d;
        _float t2 = (he - p) / d;
        if (t1 > t2)
            swap(t1, t2);
        if (t1 > tmin) 
            tmin = t1;
        if (t2 < tmax) 
            tmax = t2;
        return tmin <= tmax;
        };

    if (!slab(px, dx, b.halfExtents.x)) 
        return hit;
    if (!slab(py, dy, b.halfExtents.y))
        return hit;
    if (!slab(pz, dz, b.halfExtents.z))
        return hit;

    _float tHit = (tmin >= 0.f) ? tmin : tmax;

    if (tHit < 0.f || tHit > maxT) 
        return hit;

    hit.isHit = true;
    hit.distance = tHit * dirLen;
    hit.hitPos = _ray.origin + d * (tHit * dirLen);

    vector3 localHit(px + dx * tHit, py + dy * tHit, pz + dz * tHit);
    vector3 nLocal(0, 0, 0);
    _float bx = fabsf(localHit.x) - b.halfExtents.x;
    _float by = fabsf(localHit.y) - b.halfExtents.y;
    _float bz = fabsf(localHit.z) - b.halfExtents.z;
    _float m = max(bx, max(by, bz));

    if (m == bx)
        nLocal = vector3((localHit.x > 0) ? 1.f : -1.f, 0, 0);
    else if (m == by)
        nLocal = vector3(0, (localHit.y > 0) ? 1.f : -1.f, 0);
    else  
        nLocal = vector3(0, 0, (localHit.z > 0) ? 1.f : -1.f);

    hit.hitNormal = b.axis[0] * nLocal.x + b.axis[1] * nLocal.y + b.axis[2] * nLocal.z;
    hit.object = m_pGameObject;
    hit.collider = const_cast<CBoxCollider*>(this);

    return hit;
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

_bool CBoxCollider::IntersectOBBtoOBB(const OBB& _boxA, const OBB& _boxB, _float* _outPen, vector3* _outAxis)
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

_bool CBoxCollider::IntersectOBBtoSPHERE(const OBB& _box, const CSphereCollider::SPHERE& _sphere, _float* _outPen, vector3* _outAxis)
{
    const _float EPS = 1e-6f; 
    const vector3 d = _sphere.center - _box.center;
    const _float t0 = vector3::dot(d, _box.axis[0]); 
    const _float t1 = vector3::dot(d, _box.axis[1]);
    const _float t2 = vector3::dot(d, _box.axis[2]);
    auto clampf = [](_float v, _float lo, _float hi)->_float { return (v < lo) ? lo : (v > hi) ? hi : v; }; 
    const _float c0 = clampf(t0, -_box.halfExtents.x, _box.halfExtents.x);
    const _float c1 = clampf(t1, -_box.halfExtents.y, _box.halfExtents.y); 
    const _float c2 = clampf(t2, -_box.halfExtents.z, _box.halfExtents.z);
    const vector3 closest = _box.center + _box.axis[0] * c0 + _box.axis[1] * c1 + _box.axis[2] * c2; 
    const vector3 delta = _sphere.center - closest; 
    const _float distSq = delta.lengthSq(); 
    const _float r = _sphere.radius;
    if (distSq > r * r) 
    {
        if (_outPen)
            *_outPen = 0.f;
        if (_outAxis)
            *_outAxis = vector3::zero();
        return false;
    }
    
    if (distSq > EPS * EPS) 
    { 
        const _float dist = sqrtf(distSq);
        if (_outPen) 
            *_outPen = r - dist; 
        if (_outAxis)
            *_outAxis = delta / dist;
        return true;
    } 
    
    const _float dx = _box.halfExtents.x - fabsf(t0);
    const _float dy = _box.halfExtents.y - fabsf(t1);
    const _float dz = _box.halfExtents.z - fabsf(t2);
    _float minFace = dx;
    vector3 axis = (t0 >= 0.f) ? _box.axis[0] : -_box.axis[0];
    if (dy < minFace) 
    {
        minFace = dy; 
        axis = (t1 >= 0.f) ? _box.axis[1] : -_box.axis[1];
    } if (dz < minFace) 
    {
        minFace = dz;
        axis = (t2 >= 0.f) ? _box.axis[2] : -_box.axis[2];
    } 
    if (_outAxis)
        *_outAxis = axis;
    if (_outPen) 
    { 
        const _float pen = (minFace >= r) ? (r + minFace) : (r - minFace); *_outPen = pen; 
    }
    
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
