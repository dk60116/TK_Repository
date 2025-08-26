#include "epch.h"
#include "Physics.h"

CPhysics::CPhysics()
	: m_vGravityVector(vector3(0.f, -9.81f, 0.f))
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

vector<CPhysics::RAYCASTHIT> CPhysics::Raycast(const Ray& _ray, _uint _layerMask)
{
    vector<RAYCASTHIT> hits;

    vector<CRenderer*> renders = CSceneManager::Get_CrtScene()->Get_MeshObjects(_layerMask);

	for (auto ren : renders)
	{
		CMeshBuffer* buffer = ren->Get_MeshBuffer();

		if (!buffer)
			continue;

		vector<VertexTexNormalTangentBuffer> vb = buffer->Get_VertexBuffer();
		vector<_uint> ib = buffer->Get_IndexBuffer();

		if (vb.size() <= 0 || ib.size() <= 0)
			continue;

		CGameObject* obj = ren->Get_GameObject();
		_matrix worldMatrix = obj->Get_Transform()->Get_WorldMatrix();

		for (_uint i = 0; i < ib.size(); i += 3)
		{
			vector3 p0 = XMVector3TransformCoord(XMLoadFloat3(&vb[ib[i]].position), worldMatrix);
			vector3 p1 = XMVector3TransformCoord(XMLoadFloat3(&vb[ib[i + 1]].position), worldMatrix);
			vector3 p2 = XMVector3TransformCoord(XMLoadFloat3(&vb[ib[i + 2]].position), worldMatrix);

			_float t = 0.f;
			vector3 normal;

			if (IntersectRayTriangle(_ray.origin, _ray.dir, p0, p1, p2, t, normal))
			{
				if (t < 0 || t > _ray.maxDist)
					continue;

				RAYCASTHIT hit;
				hit.isHit = true;
				hit.distance = t;
				hit.hitNormal = normal;
				hit.hitPos = _ray.origin + _ray.dir * t;
				hit.object = obj;

				hits.push_back(hit);
			}
		}
	}

	sort(hits.begin(), hits.end(), [](const RAYCASTHIT& a, const RAYCASTHIT& b) {return a.distance < b.distance; });
     
    return hits;
}

const vector3& CPhysics::Get_Gravity()
{
	return GetInstance().m_vGravityVector;
}

void CPhysics::Set_GravityValue(const vector3 _value)
{
	GetInstance().m_vGravityVector = _value;
}

_bool CPhysics::IntersectRayTriangle(const vector3& rayOrigin, const vector3& rayDir, const vector3& v0, const vector3& v1, const vector3& v2, _float& t, vector3& hitNormal)
{
	const _float EPSILON = 0.000001f;

	vector3 edge1 = v1 - v0;
	vector3 edge2 = v2 - v0;

	vector3 h = rayDir.cross(edge2);
	_float a = edge1.dot(h);
	if (fabs(a) < EPSILON)
		return false;

	_float f = 1.0f / a;
	vector3 s = rayOrigin - v0;
	_float u = f * s.dot(h);
	if (u < 0.0f || u > 1.0f)
		return false;

	vector3 q = s.cross(edge1);
	_float v = f * rayDir.dot(q);
	if (v < 0.0f || u + v > 1.0f)
		return false;

	t = f * edge2.dot(q);
	if (t > EPSILON) 
	{
		hitNormal = edge1.cross(edge2).normalized();
		return true;
	}
	
	return false;
}