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
	const vector<CCollider*>& Get_ColliderList();
	CCollider* Add_Collider(CCollider* _collider);

private:
	vector<CCollider*> m_vColliderList;
};

NS_END

