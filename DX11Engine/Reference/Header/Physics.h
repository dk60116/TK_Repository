#pragma once

#include "epch.h"

NS_BEGIN(Engine)

class ENGINE_DLL CPhysics
{
public:
	typedef struct LaycastHitInformation
	{
		_bool isHit = false;
		vector3 hitPos = {};
		vector3 hitNormal = {};
		_float distance = 0.f;
		CGameObject* object = nullptr;
	}LAYCASTHIT;

	SINGLETONCLASS(CPhysics);

public:
	LAYCASTHIT Laycast(vector3 _worldPos, vector3 _direction, _float _maxDistance = 999999.f);
};

NS_END

