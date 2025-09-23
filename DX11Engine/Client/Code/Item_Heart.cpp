#include "cpch.h"
#include "Item_Heart.h"

CItem_Heart::CItem_Heart()
{
	m_strItemName = L"Heart";
	m_strItemDescription = L"HP increases by one point.";
}

CItem_Heart::~CItem_Heart()
{
}

void CItem_Heart::OnGetEvent(const _uint _count)
{
	CGameManager::GetInstance().Get_Player()->AddHeart();
}

void CItem_Heart::OnDropEvent(const _uint _count)
{
}

void CItem_Heart::OnUseEvent(const _uint _count)
{
}

void CItem_Heart::OnUnUseEvent(const _uint _count)
{
}
