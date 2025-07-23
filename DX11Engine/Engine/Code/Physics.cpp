#include "epch.h"
#include "Physics.h"

CPhysics::CPhysics()
{
}

CPhysics::~CPhysics()
{
}

CPhysics& CPhysics::GetInstance()
{
	static CPhysics inst;

	return inst;
}

vector<CPhysics::RAYCASTHIT> CPhysics::Raycast(const Ray& _ray)
{
    vector<RAYCASTHIT> hits;
    const vector3 dirN = _ray.dir.normalized();

    auto objs = CSceneManager::GetInstance().Get_CrtScene()->Get_MeshObjects();

    for (auto* obj : objs)
    {
        if (!obj || !obj->IsActive())
            continue;

        CMeshRenderer* renderer = obj->GetComponent<CMeshRenderer>();
        if (!renderer)
            continue;

        CMeshFilter* mf = renderer->Get_MeshFilter();
        if (!mf)
            continue;

        CMeshBuffer* buffer = mf->Get_MeshBuffer();
        if (!buffer)
            continue;

        const auto& desc = buffer->Get_Info();
        const _uint* indices = static_cast<const _uint*>(buffer->m_pIndexSysMem);
        const _float3* verts = static_cast<const _float3*>(buffer->m_pVertexSysMem);

        const _uint triCnt = desc.indexCount / 3;
        if (triCnt == 0)
            continue;

        _matrix mWorld = obj->Get_Transform()->Get_WorldMatrix();

        _float closestTHit = FLT_MAX;
        vector3 closestNormal = {};
        vector3 closestHitPos = {};
        _bool foundHit = false;

        for (_uint i = 0; i < triCnt; ++i)
        {
            _float3 p0 = verts[indices[i * 3 + 0]];
            _float3 p1 = verts[indices[i * 3 + 1]];
            _float3 p2 = verts[indices[i * 3 + 2]];

            _float3 wp0, wp1, wp2;
            XMStoreFloat3(&wp0, XMVector3TransformCoord(XMLoadFloat3(&p0), mWorld));
            XMStoreFloat3(&wp1, XMVector3TransformCoord(XMLoadFloat3(&p1), mWorld));
            XMStoreFloat3(&wp2, XMVector3TransformCoord(XMLoadFloat3(&p2), mWorld));

            _float tHit;
            vector3 nHit;
            if (IntersectRayTri(_ray, wp0, wp1, wp2, tHit, nHit) && tHit <= _ray.maxDist)
            {
                if (tHit < closestTHit)
                {
                    closestTHit = tHit;
                    closestNormal = nHit;
                    closestHitPos = _ray.origin + dirN * tHit;
                    foundHit = true;
                }
            }
        }

        if (foundHit)
        {
            RAYCASTHIT hit;
            hit.isHit = true;
            hit.distance = closestTHit;
            hit.hitPos = closestHitPos;
            hit.hitNormal = closestNormal.normalized();
            hit.object = obj;
            hits.emplace_back(hit);
        }
    }

    sort(hits.begin(), hits.end(),
        [](const RAYCASTHIT& a, const RAYCASTHIT& b) { return a.distance < b.distance; });

    return hits;
}

_bool CPhysics::IntersectRayTri(
    const Ray& ray,
    const _float3& v0F,
    const _float3& v1F,
    const _float3& v2F,
    _float& t,              
    vector3& outNormal)
{
    constexpr float EPS = 1e-6f;

    _float3 xmRayOrg3 = ray.origin;
    _float3 xmRayDir3 = ray.dir;

    const XMVECTOR orig = XMLoadFloat3(&xmRayOrg3);
    const XMVECTOR dir = XMLoadFloat3(&xmRayDir3);

    const XMVECTOR v0 = XMLoadFloat3(&v0F);
    const XMVECTOR v1 = XMLoadFloat3(&v1F);
    const XMVECTOR v2 = XMLoadFloat3(&v2F);

    const XMVECTOR e1 = XMVectorSubtract(v1, v0);
    const XMVECTOR e2 = XMVectorSubtract(v2, v0);

    const XMVECTOR p = XMVector3Cross(dir, e2);
    const float det = XMVectorGetX(XMVector3Dot(e1, p));

    if (fabsf(det) < EPS) return false;

    const float invDet = 1.f / det;

    const XMVECTOR s = XMVectorSubtract(orig, v0);  
    const float u = XMVectorGetX(XMVector3Dot(s, p)) * invDet;
    if (u < 0.f || u > 1.f) return false;

    const XMVECTOR q = XMVector3Cross(s, e1);
    const float v = XMVectorGetX(XMVector3Dot(dir, q)) * invDet;
    if (v < 0.f || (u + v) > 1.f) return false;

    t = XMVectorGetX(XMVector3Dot(e2, q)) * invDet;

    if (t < 0.f) 
        return false;   

    XMVECTOR n = XMVector3Cross(e1, e2);
    n = XMVector3Normalize(n);
    _float3 on;
    XMStoreFloat3(&on, n);

    outNormal = vector3(on.x, on.y, on.z);

    return true;
}
