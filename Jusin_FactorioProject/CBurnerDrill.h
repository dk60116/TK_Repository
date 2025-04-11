#pragma once

#include "CStructure.h"
#include "CInOutStructure.h"

class CBurnerDrill :
    public CStructure
{
public:
	CBurnerDrill();
	~CBurnerDrill();

public:
	void Awake() override;
	void Start() override;
	void Reset() override;
	void Update() override;
	void FixedUpdate() override;
	void LateUpdate() override;
	void OnEnable() override;
	void OnDisable() override;
	void Render(HDC _hDC) override;
	void OnCollisionEnter(CCollider* _other) override;
	void OnCollisionStay(CCollider* _other) override;
	void OnCollisionExit(CCollider* _other) override;
	void OnDestroy() override;
	void OnMouseEnter() override;
	void OnMouseOver() override;
	void OnMouseExit() override;

public:
	void OnOutputHandler() override;

private:
	CInOutStructure* m_pInOut;
};

