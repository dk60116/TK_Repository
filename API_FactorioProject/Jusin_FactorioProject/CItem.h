#pragma once

#include "pch.h"

class CItem;
class CItemSlot;

struct ItemInfo
{
	int id = 0;
	wstring itemName = L"";
	wstring icon_Slot = L"";
	wstring icon_Field = L"";
	vector2 fieldSize = vector2::one();;
	vector2Int blueprintSpriteArea = vector2Int::one() * 32;
	int count = 0;
	CItemSlot* slot = nullptr;
	float createDuring = 0.5f;
	int createCount = 1;
	bool isStructure = true;
};

struct IMaterial
{
	int item = -1;
	int  count = 1;
};

struct NeedItemResult
{
	vector<IMaterial> HaveItems = {};
	vector<IMaterial> NeedItems = {};
};

class CItem
{
public:
	CItem();
	CItem(CItem& _rhs);
	~CItem();

public:
	ItemInfo& getInfo() { return m_sInfo; }
	vector<IMaterial>& getMaterials() { return m_vMaterials; }
	void AddMaterial(CItem* _item, int _count);

	vector<IMaterial> getMaterialsAll();
	NeedItemResult getMaterialsOfItemList(vector<CItem*> _items);

private:
	ItemInfo m_sInfo;
	vector<IMaterial> m_vMaterials;
};

