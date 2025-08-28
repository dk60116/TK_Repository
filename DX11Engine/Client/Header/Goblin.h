#pragma once

#include "Monster.h"

class CGoblin final : public CMonster
{
private:
	explicit CGoblin();
	~CGoblin();

public:
	static CGoblin* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
};

