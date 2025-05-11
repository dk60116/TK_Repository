#pragma once

#include "CMonoBehaviour.h"
#include "CImage.h"
#include "CButton.h"
#include "CInventory.h"
#include "CItemSlot.h"
#include "CCreateItemBtn.h"
#include "CInOutStructure.h"

class CInventoryUI :
    public CMonoBehaviour
{
public:
	enum RightSideMenuType { CreateItemTable, IOStructureMenu, END };

public:
    CInventoryUI();
    ~CInventoryUI();

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
	CUI* getUI() { return m_pUI; }
	CInventory& getInven() { return *m_pInven; }	
	void SetInven(CInventory* _inven) { m_pInven = _inven; }
	void UpdateInventory(vector<CItem*> itemList);
	void UpdateCreateWindow(vector<CItem*> itemList);
	void AllOffSlot();
	void SetSideType(const RightSideMenuType _type) { m_eSideType = _type; }
	void SetIO(CInOutStructure* _io) { m_pOpenedIOStructure = _io; }
	CInOutStructure* getIO() { return m_pOpenedIOStructure; }

private:
	CInventory* m_pInven;
    CImage* m_pUI;
	bool m_bIsMove;
	vector2 m_v2Pivot;
	CButton* m_pCloseBtn;
	vector<CItemSlot*> m_vItemSlot;
	CImage* m_pRightSideMenu[RightSideMenuType::END];
	vector<CCreateItemBtn*> m_vCreateItemBtn;
	vector2Int m_v2ItemSlotSize, m_v2CIBtnSize;
	RightSideMenuType m_eSideType;
	CInOutStructure* m_pOpenedIOStructure;
	CItem* m_pIOMenuInputItem, * m_pIOMenuOutputItem, * m_pIOMenuFuelItem;
	CImage* m_pIOStructureImage;
	CItemSlot* m_pIOMenuInputSlot, * m_pIOMenuOutputSlot, * m_pIOMenuFuelSlot;
	CImage* m_pIOMenuTaskBar, * m_pIOMenuTaskGauge;
	CImage* m_pIOMenuFuelBar, * m_pIOMenuFuelGauge;
};

