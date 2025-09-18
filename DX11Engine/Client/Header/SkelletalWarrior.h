#pragma once
#include "Monster.h"

class CSkelletalWarrior final : public CMonster
{
private:
	explicit CSkelletalWarrior();
	~CSkelletalWarrior();

public:
	static CSkelletalWarrior* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize(void* _desc) override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
};

