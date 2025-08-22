#pragma once

#include "Weapon.h"

class CArrow abstract : public CWeapon
{
protected:
	CArrow();
	~CArrow();

public:
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

	void OnCollisionEnter(class CCollider* _other) override;
	void OnCollisionStay(class CCollider* _other) override;
	void OnCollisionExit(class CCollider* _other) override;

public:
	const _bool IsUsed() const;

public:
	virtual void Pop();
	virtual void Shoot();
	virtual void Return();

private:
	CRigidBody* m_pRigidBody;
	_bool m_bUsed;
	_float m_fLifeTime, m_fPassedTime;
};

