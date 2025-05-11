#pragma once

#include "pch.h"
#include "CItem.h"
#include "CDropItem.h"

class CItemManager
{
	SingletonClass(CItemManager);

public:
	void Init();
	void Release();

	CItem* CreateItem(const int _id, const wstring _name, const wstring _iconA, const wstring _iconB);
	CItem* GetItem(const int _itemId);
	map<const int, CItem*>& getItemList() { return m_mItemList; };

private:
	map<const int, CItem*> m_mItemList;
};

