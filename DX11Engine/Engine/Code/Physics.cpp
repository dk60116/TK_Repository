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

CPhysics::LAYCASTHIT CPhysics::Laycast(vector3 _worldPos, vector3 _direction, _float _maxDistance)
{
	LAYCASTHIT result;

	_float closestDist = _maxDistance;
	CGameObject* closestObject = nullptr;
	vector3 hitPoint = {};
	vector3 hitNormal = {};

	for (auto& object : CSceneManager::GetInstance().Get_CrtScene()->Get_MeshObjects())
	{
		CDebug::Log(object->Get_ObjectName());

		CMeshBuffer* mesh = nullptr;

		if (object->GetComponent<CMeshRenderer>())
			mesh = object->GetComponent<CMeshRenderer>()->Get_MeshFilter()->Get_MeshBuffer();
		else
			mesh = object->GetComponent<CSkinnedMeshRenderer>()->Get_MeshBuffer();

		if (!mesh)
			continue;

		_matrix worldMat = object->Get_Transform()->Get_WorldMatrix();

		//for (_int i = 0; i < mesh->GetIndexCount(); i += 3)
		//{
		//	vector3 v0 = TransformPosition(mesh->GetVertex(i + 0), worldMat);
		//	vector3 v1 = TransformPosition(mesh->GetVertex(i + 1), worldMat);
		//	vector3 v2 = TransformPosition(mesh->GetVertex(i + 2), worldMat);

		//	_float dist = 0.f;
		//	vector3 normal;
		//	if (RayIntersectsTriangle(_worldPos, _direction, v0, v1, v2, dist, normal))
		//	{
		//		if (dist < closestDist)
		//		{
		//			closestDist = dist;
		//			closestObject = object;
		//			hitPoint = _worldPos + _direction * dist;
		//			hitNormal = normal;
		//		}
		//	}
		}
	}

	if (closestObject)
	{
		result.isHit = true;
		result.distance = closestDist;
		result.hitPos = hitPoint;
		result.hitNormal = hitNormal;
		result.object = closestObject;
	}

	return result;
}
