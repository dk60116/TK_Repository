#pragma once
#include "Monster.h"

class CLizardWarrior final : public CMonster
{
protected:
	explicit CLizardWarrior();
	~CLizardWarrior();

public:
	static CLizardWarrior* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
};

