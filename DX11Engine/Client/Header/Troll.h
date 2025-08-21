#pragma once

#include "Monster.h"

class CTroll : public CMonster
{
protected:
	explicit CTroll();
	~CTroll();

public:
	static CTroll* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
};

