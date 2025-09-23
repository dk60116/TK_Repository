#include "cpch.h"
#include "Item.h"

CItem::CItem()
	: m_strItemName({})
	, m_strItemDescription({})
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

void CItem::OnUseEvent(const _uint _count)
{
}

void CItem::OnUnUseEvent(const _uint _count)
{
}

const wstring& CItem::Get_ItemName()
{
	return m_strItemName;
}

const wstring& CItem::Get_ItemDescription()
{
	return m_strItemDescription;
}
