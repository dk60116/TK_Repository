#pragma once

#include "CMonoBehaviour.h"
#include "CItemManager.h"

class CInventoryUI;

class CInventory :
    public CMonoBehaviour
{
public:
    CInventory();
    ~CInventory();

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
	vector<CItem*>& getItemList() { return m_vItemList; }
    void AddItem(const int _itemID, const int _count, const float _fHp = 1.f);
	void SubItem(const int _itemID, const int _count);
	void SwapItem(const int _indexA, const int _indexB);
	bool HaveItem(const int _itemID, const int _count = 1);

	CInventoryUI* getInvenUI() { return m_pInvenUI; }
	void SetIvenUI(CInventoryUI* _ui) { m_pInvenUI = _ui; }

private:
    vector<CItem*> m_vItemList;
	CInventoryUI* m_pInvenUI;
};

