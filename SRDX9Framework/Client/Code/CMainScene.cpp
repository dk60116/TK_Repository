#include "pch.h"
#include "CMainScene.h"
#include "CTriCol.h"
#include "CSpriteRenderer.h"
#include "CMeshRenderer.h"
#include "CMeshRenderer.h"

CMainScene::CMainScene()
	: m_pPlayer(nullptr)
	, m_pEnemy(nullptr)
{
}

CMainScene::~CMainScene()
{
	CScene::Destroy();
}

void CMainScene::Awake()
{
	CScene::Awake();

	m_sOptions.lighting = true;

	CGameObject* cameraObj = AddObject(L"Main Camera", Layer::DEFAULT);
	cameraObj->AddComponent<CCamera>();
	cameraObj->getTransform().SetPosition(0.f, 0.f, -10.f);

	CGameObject* lightObj = AddObject(L"MainLight", Layer::DEFAULT);
	lightObj->AddComponent<CLight>();
	lightObj->getTransform().SetEulerAngles(20.f, 30.f, 0.f);

	CGameObject* playerObj = AddObject(L"Player", Layer::DEFAULT);
	m_pPlayer = playerObj->AddComponent<CPlayer>();

	CGameObject* playerHead = AddObject(L"PlayerHead", Layer::DEFAULT);
	CSpriteRenderer* phRender = playerHead->AddComponent<CSpriteRenderer>();
	phRender->getOptions().sortOrder = 1;
	playerHead->getTransform().SetLocalPosition(0.f, 0.5f, 0.f);
	playerHead->getTransform().SetLocalScale(0.25f, 1.f, 1.f);
	playerHead->getTransform().SetParent(playerObj->getTransform());

	CGameObject* enemyObj = AddObject(L"Enemy", Layer::DEFAULT);
	m_pEnemy = enemyObj->AddComponent<CEnemy>();
	m_pEnemy->SetTarget(&m_pPlayer->getTransform());

	CGameObject* triObj = AddObject(L"Tri", Layer::DEFAULT);
	CSpriteRenderer* tSRender = triObj->AddComponent<CSpriteRenderer>();
	triObj->getTransform().SetPosition(-2.f, -2.f, -1.f);
	auto tex = CResources::GetInstance().getResource<CTexture>(L"Player").get();
	if (tex)
		tSRender->SetTexture(tex);

	CGameObject* boxObj = AddObject(L"Box", Layer::DEFAULT);
	boxObj->getTransform().SetPosition(2.f, -2.f, 0.f);
	CMeshRenderer* boxRender = boxObj->AddComponent<CMeshRenderer>();
	boxRender->SetMeshFilterType(CMesh::CUBE);
}

void CMainScene::Start()
{
}

void CMainScene::Update()
{
	CScene::Update();

	//m_vCameraList.back()->getTransform().AddLocalPosition(camDir.normalized() * DELTA_TIME);

	//m_vCameraList.back()->getTransform().SetPosition(m_pPlayer->getTransform().getPosition() + vector3::back() * 10.f);

	if (CInput::GetInstance().GetKeyDown(TWO))
		m_sOptions.lighting = !m_sOptions.lighting;

	//m_pPlayer->AddRef();
	//ULONG rcount = m_pPlayer->Release();
	//CDebug::Log((int)rcount);
}

void CMainScene::FixedUpdate()
{
}

void CMainScene::LateUpdate()
{
}

void CMainScene::Render_Game()
{
	CScene::Render_Game();
}

void CMainScene::Destroy()
{
}
