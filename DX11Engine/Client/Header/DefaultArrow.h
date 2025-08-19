#pragma once

#include "Arrow.h"

class CDefaultArrow final : public CArrow
{
public:
	explicit CDefaultArrow();
	~CDefaultArrow();

public:
	static CDefaultArrow* Create();
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

