#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CCollider abstract : public CComponent
{
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
	void OnDestroy() override;

public:
	virtual void Set_Center(const vector3 _center) PURE;
	const _bool IsTrigger() const;
	void SetTrigger(const _bool _value);
	class CRigidBody* Get_RigidBody();
	void Set_RigidBody(CRigidBody* _rigid);

public:
	void EnterOther(CCollider* _other);
	void ExitOther(CCollider* _other);

protected:
	_uint m_iColliderID;
	_bool m_bIsTrigger;
	map<_uint, CCollider*> m_mEnteredColliders;

	CRigidBody* m_pRigid;
};

NS_END

