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
	CGameObject* playerObj = Add_GameObject(L"Player");
	m_pPlayer = playerObj->AddComponent<CPlayer>();

	CGameObject* cameraObject = Add_GameObject(L"Main Camera");
	m_pMainCamera = cameraObject->AddComponent<CCamera>();

	m_pMainCamera->Get_Transform()->Set_Position(0.f, 0.f, -500.f);

	return S_OK;
}

void CMainScene::Update()
{
	__super::Update();

	if (CInput::GetInstance().GetKey(W))
		m_pPlayer->Get_Transform()->Add_PositionY(1.f * DELTA_TIME);
	if (CInput::GetInstance().GetKey(A))
		m_pPlayer->Get_Transform()->Add_PositionX(-1.f * DELTA_TIME);
	if (CInput::GetInstance().GetKey(S))
		m_pPlayer->Get_Transform()->Add_PositionY(-1.f * DELTA_TIME);
	if (CInput::GetInstance().GetKey(D))
		m_pPlayer->Get_Transform()->Add_PositionX(1.f * DELTA_TIME);

	if (CInput::GetInstance().GetKey(UP))
		m_pMainCamera->Get_Transform()->Add_PositionY(1.f * DELTA_TIME);
	if (CInput::GetInstance().GetKey(LEFT))
		m_pMainCamera->Get_Transform()->Add_PositionX(-1.f * DELTA_TIME);
	if (CInput::GetInstance().GetKey(DOWN))
		m_pMainCamera->Get_Transform()->Add_PositionY(-1.f * DELTA_TIME);
	if (CInput::GetInstance().GetKey(RIGHT))
		m_pMainCamera->Get_Transform()->Add_PositionX(1.f * DELTA_TIME);

	if (CInput::GetInstance().GetKey(O))
		m_pMainCamera->Get_Transform()->Add_EulerAngle(vector3(10.f * DELTA_TIME, 0.f, 0.f));
}
