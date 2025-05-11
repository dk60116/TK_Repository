#pragma once

#include "CUI.h"
#include "CItem.h"
#include "CImage.h"

class CCreatingSlot :
    public CUI
{
public:
	CCreatingSlot(int _index);
	~CCreatingSlot();

public:
	void UI_OnMouseEnter() override;
	void UI_OnMouseStay() override;
	void UI_OnMouseExit() override;

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
	void SetItem(CItem& _item);
	void SetStart(const bool _start) { m_bStart = _start; }

private:
	int m_iIndex;
	CItem* m_pItem;
	CImage* m_pSlotImg;
	CImage* m_pItemImg;
	CImage* m_pFillImg;
	bool m_bStart;
	float m_fDuring;
};

