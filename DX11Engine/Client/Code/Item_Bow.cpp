#include "cpch.h"
#include "Item_Bow.h"

CItem_Bow::CItem_Bow()
{
	m_strItemName = L"Bow";
	m_strItemDescription = L"Shoot the arrow";
}

CItem_Bow::~CItem_Bow()
{
}

void CItem_Bow::OnGetEvent(const _uint _count)
{
}

void CItem_Bow::OnDropEvent(const _uint _count)
{
}

void CItem_Bow::OnUseEvent()
{
}

void CItem_Bow::OnUnUseEvent()
{
}
