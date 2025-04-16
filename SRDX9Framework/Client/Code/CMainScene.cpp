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

	m_sOptions.lighting = true;

	CGameObject* cameraObj = AddObject(L"Main Camera", Layer::DEFAULT);
	cameraObj->AddComponent<CCamera>();
	cameraObj->getTransform().SetPosition(0.f, 0.f, -5.f);

	CGameObject* lightObj = AddObject(L"MainLight", Layer::DEFAULT);
	lightObj->AddComponent<CLight>();

	 CGameObject* playerObj = AddObject(L"Player", Layer::DEFAULT);
	m_pPlayer = playerObj->AddComponent<CPlayer>();

	CGameObject* playerHead = AddObject(L"PlayerHead", Layer::DEFAULT);
	CSpriteRenderer* phRender = playerHead->AddComponent<CSpriteRenderer>();
	phRender->SetSortOrder(1);
	playerHead->getTransform().SetLocalPosition(0.f, 0.5f, 0.f);
	playerHead->getTransform().SetLocalScale(0.25f, 1.f, 1.f);
	playerHead->getTransform().SetParent(playerObj->getTransform());

	CGameObject* enemyObj = AddObject(L"Enemy", Layer::DEFAULT);
	m_pEnemy = enemyObj->AddComponent<CEnemy>();
	m_pEnemy->SetTarget(&m_pPlayer->getTransform());

	CGameObject* boxObj = AddObject(L"Box", Layer::DEFAULT);
	CSpriteRenderer* boxRender = boxObj->AddComponent<CSpriteRenderer>();
	boxObj->getTransform().SetPosition(-2.f, -2.f, -1.f);
	auto tex = CResources::GetInstance().getResource<CTexture>(L"Player").get();
	if (tex)
		boxRender->SetTexture(tex);
}

void CMainScene::Start()
{
}

void CMainScene::Update()
{
	CScene::Update();

	vector3 camDir = vector3::zero();

	if (CInput::GetInstance().GetKey(UP))
		camDir.y += 1.f;
	if (CInput::GetInstance().GetKey(DOWN))
		camDir.y += -1.f;
	if (CInput::GetInstance().GetKey(LEFT))
		camDir.x += -1.f;
	if (CInput::GetInstance().GetKey(RIGHT))
		camDir.x += 1.f;

	if (m_vCameraList.empty())
		return;

	m_vCameraList.back()->getTransform().SetPosition(m_pPlayer->getTransform().getPosition() + vector3::back() * 5.f);

	if (CInput::GetInstance().GetKeyDown(TWO))
		m_sOptions.lighting = !m_sOptions.lighting;
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
