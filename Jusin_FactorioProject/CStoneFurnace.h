#pragma once

#include "CStructure.h"
#include "CAnimator.h"

class CInOutStructure;

class CStoneFurnace :
    public CStructure
{
public:
	CStoneFurnace();
	~CStoneFurnace();

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
	CInOutStructure* getIO() { return m_pInOut; }

public:
	void OnOutputHandler() override;

private:
	CInOutStructure* m_pInOut;
	CAnimator* m_pAnimator;
	CGameObject* m_pUpFireObj, * m_pFireAnimObj;
};

