#include "cpch.h"
#include "MainScene.h"
#include "Player.h"

CMainScene::CMainScene()
	: CScene{}
	, m_pMainCamera(nullptr)
	, m_pPlayer(nullptr)
{
}

CMainScene::~CMainScene()
{
}

HRESULT CMainScene::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	CGameObject* playerObj = Add_GameObject(L"Player");
	m_pPlayer = playerObj->AddComponent<CPlayer>();

	CGameObject* cameraObject = Add_GameObject(L"Main Camera");
	m_pMainCamera = cameraObject->AddComponent<CCamera>();

	m_pMainCamera->Get_Transform()->Set_Position(0.f, 0.f, -10.f);

	return S_OK;
}

void CMainScene::Update()
{
	__super::Update();
}
