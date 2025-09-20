#include "cpch.h"
#include "Inventory.h"

#include "Item_Heart.h"
#include "Item_DungeonKey.h"
#include "Item_Sword.h"
#include "Item_Bow.h"
#include "Item_Arrow.h"

CInventory::CInventory()
	: m_mItemList({})
{
}

CInventory::~CInventory()
{
}

CInventory* CInventory::Create()
{
	return new CInventory();
}

CComponent* CInventory::Clone() const
{
	CInventory* clone = new CInventory();

	return clone;
}

HRESULT CInventory::Initialize(void* _desc)
{
	if (FAILED(__super::Initialize(_desc)))
		E_FAIL;

	Add_ItemSlot<CItem_Heart>();
	Add_ItemSlot<CItem_DungeonKey>();
	Add_ItemSlot<CItem_Sword>();
	Add_ItemSlot<CItem_Bow>();
	Add_ItemSlot<CItem_Arrow>();

	return S_OK;
}

void CInventory::Awake()
{
}

void CInventory::Update()
{
}

void CInventory::OnDestroy()
{
	for (TRAVERSAL_ITER(m_mItemList, it))
	{
		Safe_Release((*it).second.protoType);
	}
}

CItem* CInventory::FindItem(const wstring& _name) const
{
	auto it = m_mItemList.find(_name);
	if (it == m_mItemList.end())
		return nullptr;

	return (*it).second.protoType;
}

void CInventory::AddItem(const wstring& _name, const _uint _count)
{
	auto it = m_mItemList.find(_name);
	if (it == m_mItemList.end())
		return;

	it->second.count += _count;
	it->second.protoType->OnGetEvent(it->second.count);
}

_bool CInventory::UseItem(const wstring& _name, const _uint _count)
{
	auto it = m_mItemList.find(_name);
	if (it == m_mItemList.end())
		return false;

	if (it->second.count > 0)
	{
		it->second.count -= _count;
		it->second.protoType->OnDropEvent(it->second.count);
		return true;
	}
	else
		return false;

	return false;
}

const _bool CInventory::HasItem(const wstring& _name) const
{
	auto it = m_mItemList.find(_name);
	if (it == m_mItemList.end())
		return false;

	_bool result = (*it).second.count > 0;

	return result;
}
