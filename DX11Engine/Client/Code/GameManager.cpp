#include "cpch.h"
#include "GameManager.h"
#include "Player.h"

CGameManager::CGameManager()
	: m_strNextScene(L"")
	, m_pPlayer(nullptr)
	, m_pPlayerHUD(nullptr)
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

HRESULT CGameManager::Initialize()
{
	CSceneManager::Add_Layer(3, L"Player");
	CSceneManager::Add_Layer(4, L"PlayerWeapon");
	CSceneManager::Add_Layer(5, L"Monster");

	CCollisionManager::Set_CollisionFilter(L"Player", L"PlayerWeapon", false);

	return S_OK;
}

void CGameManager::Set_NexScene(const wstring _scneName)
{
	m_strNextScene = _scneName;
}

const wstring& CGameManager::Get_NextScene() const
{
	return m_strNextScene;
}

void CGameManager::Set_Player(CPlayer* _player)
{
	m_pPlayer = _player;
}

CPlayer* CGameManager::Get_Player()
{
	return m_pPlayer;
}

void CGameManager::Set_PlayerHUD(CPlayerHUD* _hud)
{
	m_pPlayerHUD = _hud;
}

CPlayerHUD* CGameManager::Get_PlayerHUD()
{
	return m_pPlayerHUD;
}
