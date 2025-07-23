#pragma once

#include "epch.h"

NS_BEGIN(Engine)

class ENGINE_DLL CPhysics
{
public:
	struct Ray 
	{ 
		vector3 origin; 
		vector3 dir; 
		_float maxDist = 999999.f;
	};

	typedef struct RaycastHitInformation
	{
		_bool isHit = false;
		vector3 hitPos = {};
		vector3 hitNormal = {};
		_float distance = 0.f;
		CGameObject* object = nullptr;
	}RAYCASTHIT;

	SINGLETONCLASS(CPhysics);

public:
	vector<RAYCASTHIT> Raycast(const Ray& _ray);

private:
	_bool IntersectRayTri
	(
		const Ray& ray,
		const _float3& v0F,
		const _float3& v1F,
		const _float3& v2F,
		_float& t,
		vector3& outNormal
	);
};

NS_END

