#pragma once
#include "Component.h"
#include "Item.h"

class CInventory : public CComponent
{
	friend class CGameObjct;

public:
	struct InvenItem
	{
		CItem* protoType = nullptr;
		_uint count = 0;
	};

private:
	explicit CInventory();
	~CInventory();

public:
	static CInventory* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Update() override;
	void OnDestroy() override;

public:
	void AddItem(wstring& _name, const _uint _count = 1);
	_bool UseItem(wstring& _name, const _uint _count = 1);

private:
	template<typename T>
	CItem* Add_ItemSlot();

	map<wstring, InvenItem> m_mItemList;
};

template<typename T>
inline CItem* CInventory::Add_ItemSlot()
{
	T* item = new T();

	InvenItem itemSlot = {};
	itemSlot.protoType = item;
	itemSlot.count = 1;
	
	const wstring& name = item->Get_ItemName();

	m_mItemList.emplace(name, itemSlot);

	return item;
}
