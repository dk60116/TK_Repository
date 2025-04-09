#include "pch.h"
#include "CMainScene.h"
#include "CTriCol.h"
#include "CSpriteRenderer.h"

CMainScene::CMainScene()
	: m_pPlayer(nullptr)
{
}

CMainScene::~CMainScene()
{
	CScene::Release();
}

void CMainScene::Awake()
{
	CScene::Awake();

#ifdef  _DEBUG
	CDebug::Init();
#endif

	CGameObject* cameraObj = new CGameObject(L"Main Camera", CManagement::GetInstance().getGraphicDevice());
	m_vCameraList.push_back(cameraObj->AddComponent<CCamera>());
	AddObject(cameraObj, Layer::DEFAULT);

	CGameObject* playerObj = new CGameObject(L"Player", CManagement::GetInstance().getGraphicDevice());
	m_pPlayer = playerObj->AddComponent<CPlayer>();
	AddObject(playerObj, Layer::DEFAULT);

	CGameObject* playerHead = new CGameObject(L"PlayerHead", CManagement::GetInstance().getGraphicDevice());
	playerHead->AddComponent<CSpriteRenderer>();
	AddObject(playerHead, Layer::DEFAULT);
	playerHead->getTransform().SetPosition(0.f, 0.5f, 0.f);
	playerHead->getTransform().SetScale(0.25f, 1.f, 1.f);
	playerHead->getTransform().SetParent(playerObj->getTransform());

	m_vCameraList.back()->getTransform().SetPosition(0.f, 0.f, -10.f);
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
