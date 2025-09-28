#include "cpch.h"
#include "GameScene.h"
#include "Player.h"
#include "PlayerCamera.h"
#include "PlayerHUD.h"
#include "Dungeon.h"

CGameScene::CGameScene()
	: m_pPlayerCamera(nullptr)
	, m_pDirLight(nullptr)
	, m_pPlayer(nullptr)
	, m_pHUD(nullptr)
	, m_pDungeon(nullptr)
	, m_pBGMSource(nullptr)
{
}

CGameScene::~CGameScene()
{
}

HRESULT CGameScene::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	CGameObject* cameraObject = Add_GameObject(L"Player Camera");
	m_pPlayerCamera = cameraObject->AddComponent<CPlayerCamera>();

	CGameObject* lightObject = Add_GameObject(L"Directional Light");
	m_pDirLight = lightObject->AddComponent<CLight>();
	m_pDirLight->Get_Transform()->Set_EulerAngles(25.f, 20.f, 0.f);
	m_pDirLight->Set_Intensity(0.65f);

	CGameObject* hudObject = Add_GameObject(L"Player HUD");
	m_pHUD = hudObject->AddComponent<CPlayerHUD>();

	CGameObject* playerObj = Add_GameObject(L"Player");
	m_pPlayer = playerObj->AddComponent<CPlayer>();

	CGameObject* dungeonObj = Add_GameObject(L"Dungeon");
	m_pDungeon = dungeonObj->AddComponent<CDungeon>();

	CGameObject* bgmObj = Add_GameObject(L"BGM Source");
	m_pBGMSource = dungeonObj->AddComponent<CAudioSource>();
	m_pBGMSource->SetLoop(true);

	m_pBGMSource->SetClip(CResources::LoadOnScene<CAudioClip>(L"Dungeon_MainBGM (Audio)"));

	return S_OK;
}

void CGameScene::Awake()
{
	__super::Awake();

	m_pBGMSource->Play();
}

void CGameScene::Update()
{
	__super::Update();

	if (CInput::GetMouseButtonDown(1))
		ChangeBGM(L"Dungeon_MainBGM");

	//if (CInput::GetKeyDown(RETURN))
	//	CSceneManager::LoadScene(L"Main Scene");
}

void CGameScene::SceneRelease()
{
	__super::SceneRelease();
}

void CGameScene::ChangeBGM(const wstring& _clip)
{
	auto clip = CResources::LoadOnScene<CAudioClip>(_clip + L" (Audio)");

	if (clip == m_pBGMSource->Get_Clip())
		return;

	//m_pBGMSource->SetClip(clip);
	//m_pBGMSource->Play();
}
