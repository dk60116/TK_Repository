#pragma once

#include "epch.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

NS_BEGIN(Engine)

class ENGINE_DLL CCollisionManager final
{
	SINGLETONCLASS(CCollisionManager);

public:
	static HRESULT Initialize();
	static void Release();

public:
	static void UpdateCollision();

public:
	static const vector<CCollider*>& Get_ColliderList();
	static CCollider* Add_Collider(CCollider* _collider);
	static void Set_CollisionFilter(const _uint _layerA, const _uint _layerB, const _bool _isCollidable);
	static void Set_CollisionFilter(const wstring _layerA, const wstring _layerB, const _bool _isCollidable);

	static pair<ColorValue, ColorValue> Get_GizmoColorPair(const CCollider::GizmoColor _color);

private:
	vector<CCollider*> m_vColliderList;

	map<pair<_uint, _uint>, _bool> m_mCollisionFilter;
	map<CCollider::GizmoColor, pair<ColorValue, ColorValue>> m_mColliderGizmoColorSet;
};

NS_END

