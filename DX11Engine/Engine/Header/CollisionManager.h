#pragma once

#include "epch.h"
#include "BoxCollider.h"

NS_BEGIN(Engine)

class ENGINE_DLL CCollisionManager final
{
	SINGLETONCLASS(CCollisionManager);

public:
	HRESULT Initialize();
	void Release();

public:
	void UpdateCollision();

public:
	static const vector<CCollider*>& Get_ColliderList();
	static CCollider* Add_Collider(CCollider* _collider);
	static void Set_CollisionFilter(const _uint _layerA, const _uint _layerB, const _bool _isCollidable);
	static void Set_CollisionFilter(const wstring _layerA, const wstring _layerB, const _bool _isCollidable);

private:
	vector<CCollider*> m_vColliderList;

	map<pair<_uint, _uint>, _bool> m_mCollisionFilter;
};

NS_END

