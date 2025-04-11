#pragma once

#include "CUI.h"
#include "CImage.h"
#include "CText.h"
#include "CItem.h"

class CInventoryUI;

class CItemSlot :
    public CUI
{
public:
	enum ItemSlotType { INVEN, QUICKSLOT, INPUT, OUTPUT, FUEL };

public:
	CItemSlot(int _index);
	~CItemSlot();

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
	void OnCollisionEnter(CCollider*) override;
	void OnCollisionStay(CCollider*) override;
	void OnCollisionExit(CCollider*) override;
	void OnDestroy() override;
	void OnMouseEnter() override;
	void OnMouseOver() override;
	void OnMouseExit() override;

public:
	const ItemSlotType& getType() { return m_eSlotType; }
	void SetType(const ItemSlotType _type) { m_eSlotType = _type; }
	void OpenSlot(CItem* _item);
	void CloseSlot();
	void AbleSlot();
	void DisableSlot();

private:
	void SetDefaultSprite();
	void SetHighlightSprite();
	void SetHighlightClckSprite();
	void SetSpriteHand();
	void SetSpriteHandHighlight();

private:
	ItemSlotType m_eSlotType;
	int m_iIndex;
	CItem* m_pItem;
	CImage* m_pSlotImg;
	CImage* m_pItemImg;
	CText* m_pCountTxt;
	bool m_bIsSelect, m_bIsSelectNext, m_bSwapNext;
	bool m_bAble;

public:
	static bool m_bInputNext;

	friend class CInventoryUI;
};

