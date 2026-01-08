#pragma once
#include "Weapon.h"

class CWoodenSword final : public CWeapon
{
protected:
	explicit CWoodenSword();
	~CWoodenSword();

public:
	static CWoodenSword* Create();
	CComponent* Clone() const override;

	void PlaySwingSound();

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

