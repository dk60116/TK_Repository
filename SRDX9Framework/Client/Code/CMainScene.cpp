#include "pch.h"
#include "CMainScene.h"
#include "TriCol.h"
#include "SpriteRenderer.h"
#include "MeshRenderer.h"
#include "MeshRenderer.h"

CMainScene::CMainScene()
	: m_pPlayer(nullptr)
	, m_pEnemy(nullptr)
{
}

CMainScene::~CMainScene()
{
	Destroy();
}

void CMainScene::EditorInit()
{
	__super::EditorInit();

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
	playerHead->getTransform().SetParent(&playerObj->getTransform());

	CGameObject& enemyObj = *AddObject(L"Enemy", Layer::DEFAULT);
	m_pEnemy = enemyObj.AddComponent<CEnemy>();
	m_pEnemy->SetTarget(&m_pPlayer->getTransform());

	CGameObject& triObj = *AddObject(L"Tri", Layer::DEFAULT);
	CSpriteRenderer* tSRender = triObj.AddComponent<CSpriteRenderer>();
	triObj.getTransform().SetPosition(-2.f, -2.f, -1.f);
	auto tex = CResources::GetInstance().getResource<CTexture>(L"Player").get();
	if (tex)
		tSRender->SetTexture(tex);

	CGameObject* boxObj = AddObject(L"Box", Layer::DEFAULT);
	boxObj->getTransform().SetPosition(2.f, -2.f, 0.f);
	CMeshRenderer* boxRender = boxObj->AddComponent<CMeshRenderer>();
	boxRender->SetMeshFilterType(CMesh::CUBE);
	boxRender->getTransform().SetParent(&enemyObj.getTransform());
}

void CMainScene::Awake()
{
	__super::Awake();
}

void CMainScene::Start()
{
}

void CMainScene::Update()
{
	CScene::Update();

	if (m_lCameraList.empty())
		return;

	m_lCameraList.back()->getTransform().SetPosition(m_pPlayer->getTransform().getPosition() + vector3::back() * 10.f);

	if (CInput::GetInstance().GetKeyDown(Alpha2))
		m_sOptions.lighting = !m_sOptions.lighting;

	if (CInput::GetInstance().GetKeyDown(Alpha3))
		m_pEnemy->getObject()->DestroyThis();

	if (CInput::GetInstance().GetKeyDown(Alpha4))
		CGameObject::Find(L"Box")->getTransform().SetParent(nullptr);
}

void CMainScene::FixedUpdate()
{
}

void CMainScene::LateUpdate()
{
	__super::LateUpdate();
}

void CMainScene::Render_Game()
{
	__super::Render_Game();

	CScene::Render_Game();
}

void CMainScene::Destroy()
{
	__super::Destroy();
}
