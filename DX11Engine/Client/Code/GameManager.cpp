#include "cpch.h"
#include "GameManager.h"
#include "Player.h"
#include "PlayerController.h"

CGameManager::CGameManager()
	: m_strNextScene(L"")
	, m_pPlayer(nullptr)
	, m_pPlayerCamera(nullptr)
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
	CSceneManager::Add_Layer(6, L"MonsterBodyPart");
	CSceneManager::Add_Layer(7, L"Map");
	CSceneManager::Add_Layer(8, L"DungeonChapter");

	CSceneManager::Add_Tag(1, L"Floor");
	CSceneManager::Add_Tag(2, L"Wall");
	CSceneManager::Add_Tag(3, L"Player");

	CCollisionManager::Set_CollisionFilter(L"Player", L"PlayerWeapon", false);
	CCollisionManager::Set_CollisionFilter(L"PlayerWeapon", L"PlayerWeapon", false);
	CCollisionManager::Set_CollisionFilter(L"Monster", L"PlayerWeapon", false);
	CCollisionManager::Set_CollisionFilter(L"MonsterBodyPart", L"Default", false);
	CCollisionManager::Set_CollisionFilter(L"MonsterBodyPart", L"MonsterBodyPart", false);
	CCollisionManager::Set_CollisionFilter(L"Monster", L"MonsterBodyPart", false);

	CCollisionManager::Set_CollisionFilter(L"Map", L"Map", false);
	CCollisionManager::Set_CollisionFilter(L"PlayerWeapon", L"Map", false);
	CCollisionManager::Set_CollisionFilter(L"MonsterBodyPart", L"Map", false);

	CCollisionManager::Set_CollisionFilter(L"DungeonChapter", L"Default", false);
	CCollisionManager::Set_CollisionFilter(L"DungeonChapter", L"DungeonChapter", false);
	CCollisionManager::Set_CollisionFilter(L"DungeonChapter", L"PlayerWeapon", false);
	CCollisionManager::Set_CollisionFilter(L"DungeonChapter", L"MonsterBodyPart", false);
	CCollisionManager::Set_CollisionFilter(L"DungeonChapter", L"Monster", false);

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

void CGameManager::Set_PlayerCamera(CPlayerCamera* _cam)
{
	m_pPlayerCamera = _cam;
}

CPlayerCamera* CGameManager::Get_PlayerCamera()
{
	return m_pPlayerCamera;
}

void CGameManager::Set_PlayerHUD(CPlayerHUD* _hud)
{
	m_pPlayerHUD = _hud;
}

CPlayerHUD* CGameManager::Get_PlayerHUD()
{
	return m_pPlayerHUD;
}

void CGameManager::Set_Inventory(CInventory* _inven)
{
	m_pInventory = _inven;
}

CInventory* CGameManager::Get_Inventory()
{
	return m_pInventory;
}
