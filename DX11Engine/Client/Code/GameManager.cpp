#include "cpch.h"
#include "GameManager.h"

CGameManager::CGameManager()
	: m_strNextScene(L"")
{
}

CGameManager::~CGameManager()
{
}

CGameManager& CGameManager::GetInstance()
{
	static CGameManager instance;
	return instance;
}

void CGameManager::Set_NexScene(const wstring _scneName)
{
	m_strNextScene = _scneName;
}

const wstring& CGameManager::Get_NextScene() const
{
	return m_strNextScene;
}
