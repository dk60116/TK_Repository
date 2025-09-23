#include "cpch.h"
#include "Item_DungeonKey.h"

CItem_DungeonKey::CItem_DungeonKey()
{
	m_strItemName = L"Dungeon Key";
	m_strItemDescription = L"Can open the locked gate.";
}

CItem_DungeonKey::~CItem_DungeonKey()
{
}

void CItem_DungeonKey::OnGetEvent(const _uint _count)
{
	CGameManager::GetInstance().Get_PlayerHUD()->Update_DungeonKey(_count);
}

void CItem_DungeonKey::OnDropEvent(const _uint _count)
{
	CGameManager::GetInstance().Get_PlayerHUD()->Update_DungeonKey(_count);
}

void CItem_DungeonKey::OnUseEvent(const _uint _count)
{
}

void CItem_DungeonKey::OnUnUseEvent(const _uint _count)
{
}
