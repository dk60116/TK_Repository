#pragma once

#include "pch.h"
#include "CCollider.h"
#include "CGameObject.h"

class CPhysics
{
	SingletonClass(CPhysics);

public:
	void Init();
	void Update();
	void FixedUpdate();
	void LateUpdate();
	void Release();

public:
	CCollider* AddCollider(CCollider* _collider, Layer _layer);
	void SetPhysicsLayer(const Layer _left, const Layer _right, bool _value);
	void DeleteCollider(CCollider* _collider, Layer _layer);

	bool CheckCollision_RR(CCollider& _left, CCollider& _right);

private:
	bool m_arrPhysicsLayer[Layer::END][Layer::END];
	list<CCollider*> m_lColliderList[Layer::END];
};

