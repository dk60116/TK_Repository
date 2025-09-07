#include "cpch.h"
#include "Inventory.h"
#include "Item_DungeonKey.h"

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

HRESULT CInventory::Initialize()
{
	if (FAILED(__super::Initialize()))
		E_FAIL;

	Add_ItemSlot<CItem_DungeonKey>();

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

void CInventory::AddItem(wstring& _name, const _uint _count)
{
	auto it = m_mItemList.find(_name);
	if (it == m_mItemList.end())
		return;

	it->second.count += _count;
	it->second.protoType->OnGetEvent(it->second.count);
}

_bool CInventory::UseItem(wstring& _name, const _uint _count)
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
