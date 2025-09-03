#include "cpch.h"
#include "Item.h"

CItem::CItem()
{
}

CItem::~CItem()
{
}

void CItem::OnGetEvent(const _uint _count)
{
}

void CItem::OnDropEvent(const _uint _count)
{
}

void CItem::OnUseEvent()
{
}

void CItem::OnUnUseEvent()
{
}

const wstring& CItem::Get_ItemName()
{
	return m_strItemName;
}
