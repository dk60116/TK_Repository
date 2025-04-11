#pragma once

#include "CMonoBehaviour.h"
#include "CUI.h"

class CCanvas :
    public CMonoBehaviour
{
public:
	CCanvas();
	~CCanvas();

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
	void InsertUI(CUI* _ui);

private:
	list<CUI*> m_lUiList;
};

