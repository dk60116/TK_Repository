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
};

