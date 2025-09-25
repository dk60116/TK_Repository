#pragma once
#include "Component.h"
#include "Item.h"

class CInventory : public CComponent
{
	friend class CGameObject;

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
	HRESULT Initialize(void* _desc) override;

	void Awake() override;
	void Update() override;
	void OnDestroy() override;

public:
	CItem* FindItem(const wstring& _name) const;
	void AddItem(const wstring& _name, const _uint _count = 1);
	_bool UseItem(const wstring& _name, const _uint _count = 1);
	const _bool HasItem(const wstring& _name) const;

private:
	template<typename T>
	CItem* Add_ItemSlot();

	map<wstring, InvenItem> m_mItemList;
};

template<typename T>
inline CItem* CInventory::Add_ItemSlot()
{
	T* item = new T();

	if (item)
	{
		item->AddRef();

		InvenItem itemSlot = {};
		itemSlot.protoType = item;
		itemSlot.count = 10;

		const wstring& name = item->Get_ItemName();

		m_mItemList.emplace(name, itemSlot);
	}

	return item;
}
