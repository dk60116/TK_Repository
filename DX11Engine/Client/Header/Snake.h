#pragma once
#include "Monster.h"

class CSnake final : public CMonster
{
protected:
	explicit CSnake();
	~CSnake();

public:
	static CSnake* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize(void* _desc) override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
};

