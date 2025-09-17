#pragma once
#include "Weapon.h"

class CWoodenBow final : public CWeapon
{
protected:
	explicit CWoodenBow();
	~CWoodenBow();

public:
	static CWoodenBow* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize(void* _desc) override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

	void OnCollisionEnter(class CCollider* _other) override;
	void OnCollisionStay(class CCollider* _other) override;
	void OnCollisionExit(class CCollider* _other) override;
};

