#include "pch.h"
#include "CMainScene.h"
#include "CTriCol.h"
#include "CSpriteRenderer.h"

CMainScene::CMainScene()
	: m_pPlayer(nullptr)
	, m_pEnemy(nullptr)
{
}

CMainScene::~CMainScene()
{
	CScene::Release();
}

void CMainScene::Awake()
{
	CScene::Awake();

	auto device = CManagement::GetInstance().getGraphicDevice();

	CGameObject* cameraObj = AddObject(device, L"Main Camera", Layer::DEFAULT);
	m_vCameraList.push_back(cameraObj->AddComponent<CCamera>());
	m_vCameraList.back()->getTransform().SetPosition(0.f, 0.f, -10.f);

	 CGameObject* playerObj = AddObject(device, L"Player", Layer::DEFAULT);
	m_pPlayer = playerObj->AddComponent<CPlayer>();

	CGameObject* playerHead = AddObject(device, L"PlayerHead", Layer::DEFAULT);
	playerHead->AddComponent<CSpriteRenderer>();
	playerHead->getTransform().SetPosition(0.f, 0.5f, 0.f);
	playerHead->getTransform().SetScale(0.25f, 1.f, 1.f);
	playerHead->getTransform().SetParent(playerObj->getTransform());

	CGameObject* enemy = AddObject(device, L"Enemy", Layer::DEFAULT);
	m_pEnemy = enemy->AddComponent<CEnemy>();
	m_pEnemy->SetTarget(&m_pPlayer->getTransform());
}

void CMainScene::Start()
{
}

void CMainScene::Update()
{
	CScene::Update();
}

void CMainScene::FixedUpdate()
{
}

void CMainScene::LateUpdate()
{
}

void CMainScene::Render()
{
	CScene::Render();
}

void CMainScene::Release()
{
#ifdef  _DEBUG
	CDebug::Release();
#endif
}
