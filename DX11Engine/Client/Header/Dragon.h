#pragma once
#include "BossMonster.h"

class CDragon final : public CBossMonster
{
protected:
	explicit CDragon();
	~CDragon();

public:
	static CDragon* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnEnable() override;
	void OnCollisionEnter(class CCollider* _other) override;
	void OnDestroy() override;
};

