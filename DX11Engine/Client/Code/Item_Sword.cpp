#include "cpch.h"
#include "Item_Sword.h"

CItem_Sword::CItem_Sword()
{
	m_strItemName = L"Sword";
}

CItem_Sword::~CItem_Sword()
{
}

void CItem_Sword::OnGetEvent(const _uint _count)
{
	CGameManager::GetInstance().Get_Player()->ChangeWeapon(L"Sword");
}

void CItem_Sword::OnDropEvent(const _uint _count)
{
}

void CItem_Sword::OnUseEvent()
{
}

void CItem_Sword::OnUnUseEvent()
{
}
