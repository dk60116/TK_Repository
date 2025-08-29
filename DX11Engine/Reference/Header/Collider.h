#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CCollider abstract : public CComponent
{
	friend class CCollisionManager;

public:
	enum class GizmoColor { GreenRed, BlackWhite, BlueOrange, Transparent };

public:
	typedef struct ColliderTransform
	{
		vector3 center = {};
	}COLLIDERTF;

protected:
	explicit CCollider();
	~CCollider();

public:
	HRESULT Initialize() override;
	void LateUpdate() override;
	void FixedUpdate() override;
	void OnEnable() override;
	void OnDisable() override;
	void OnDestroy() override;

public:
	virtual void Set_Center(const vector3 _center) PURE;
	const _bool IsTrigger() const;
	void SetTrigger(const _bool _value);
	class CRigidBody* Get_RigidBody();
	void Set_RigidBody(CRigidBody* _rigid);

	const map<_uint, CCollider*>& Get_EnteredColliders();

public:
	void EnterOther(CCollider* _other);
	void ExitOther(CCollider* _other);

public:
	void Set_SmoothTranslation(const _bool _value);

public:
	void Set_GizmoColor(const GizmoColor _color);

protected:
	_uint m_iColliderID;
	_bool m_bIsTrigger;
	_bool m_bSmoothCollision;
	map<_uint, CCollider*> m_mEnteredColliders;

	CRigidBody* m_pRigid;

	GizmoColor m_eGizmoColor;
};

NS_END

