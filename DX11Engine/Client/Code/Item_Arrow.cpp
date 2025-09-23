#include "cpch.h"
#include "Item_Arrow.h"

CItem_Arrow::CItem_Arrow()
{
	m_strItemName = L"Arrow";
	m_strItemDescription = L"Fire with the bow.";
}

CItem_Arrow::~CItem_Arrow()
{
}

void CItem_Arrow::OnGetEvent(const _uint _count)
{
	CGameManager::GetInstance().Get_PlayerHUD()->Update_EquipSlot(0, _count);
}

void CItem_Arrow::OnDropEvent(const _uint _count)
{
}

void CItem_Arrow::OnUseEvent(const _uint _count)
{
	CGameManager::GetInstance().Get_PlayerHUD()->Update_EquipSlot(0, _count);
}

void CItem_Arrow::OnUnUseEvent(const _uint _count)
{
}
