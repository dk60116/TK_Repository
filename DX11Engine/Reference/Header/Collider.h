#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CCollider abstract : public CComponent
{
protected:
	explicit CCollider();
	~CCollider();

public:
	HRESULT Initialize() override;
	void Awake() override;
	void LateUpdate() override;
	void FixedUpdate() override;
	void Render() override;
	void OnDestroy() override;

public:
	virtual void Set_Center(vector3 _center) PURE;

public:
	void EnterOther(CCollider* _other);
	void ExitOther(CCollider* _other);

protected:
	_uint m_iColliderID;
	map<_uint, CCollider*> m_mEnteredColliders;
};

NS_END

