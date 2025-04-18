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

	//return;

	m_sOptions.lighting = true;

	CGameObject* cameraObj = AddObject(L"Main Camera", Layer::DEFAULT);
	cameraObj->AddComponent<CCamera>();
	cameraObj->getTransform().SetPosition(0.f, 0.f, -10.f);

	//return;

	CGameObject* lightObj = AddObject(L"MainLight", Layer::DEFAULT);
	lightObj->AddComponent<CLight>();
	lightObj->getTransform().SetEulerAngles(20.f, 30.f, 0.f);

	 CGameObject* playerObj = AddObject(L"Player", Layer::DEFAULT);
	m_pPlayer = playerObj->AddComponent<CPlayer>();

	return;

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
	CMeshFilter* filter = boxObj->AddComponent<CMeshFilter>();
	filter->SetMesh(CMesh::CUBE);
	boxRender->SetMeshFilter(filter);
}

void CMainScene::Start()
{
}

void CMainScene::Update()
{
	CScene::Update();

	vector3 camDir = vector3::zero();

	if (CInput::GetInstance().GetKey(W))
		m_vCameraList.back()->getTransform().AddLocalPosition(m_vCameraList.back()->getTransform().getDirections().forward * DELTA_TIME);
	if (CInput::GetInstance().GetKey(S))
		m_vCameraList.back()->getTransform().AddLocalPosition(m_vCameraList.back()->getTransform().getDirections().back * DELTA_TIME);
	if (CInput::GetInstance().GetKey(A))
		m_vCameraList.back()->getTransform().AddLocalPosition(m_vCameraList.back()->getTransform().getDirections().left * DELTA_TIME);
	if (CInput::GetInstance().GetKey(D))
		m_vCameraList.back()->getTransform().AddLocalPosition(m_vCameraList.back()->getTransform().getDirections().right * DELTA_TIME);
	if (CInput::GetInstance().GetKey(Q))
		m_vCameraList.back()->getTransform().AddLocalPosition(m_vCameraList.back()->getTransform().getDirections().down * DELTA_TIME);
	if (CInput::GetInstance().GetKey(E))
		m_vCameraList.back()->getTransform().AddLocalPosition(m_vCameraList.back()->getTransform().getDirections().up * DELTA_TIME);

	if (CInput::GetInstance().GetKey(LEFT))
		m_vCameraList.back()->getTransform().AddLocalYAxis(-45.f * DELTA_TIME);
	if (CInput::GetInstance().GetKey(RIGHT))
		m_vCameraList.back()->getTransform().AddLocalYAxis(45.f * DELTA_TIME);
	if (CInput::GetInstance().GetKey(UP))
		m_vCameraList.back()->getTransform().AddLocalXAxis(-45.f * DELTA_TIME);
	if (CInput::GetInstance().GetKey(DOWN))
		m_vCameraList.back()->getTransform().AddLocalXAxis(45.f * DELTA_TIME);

	//m_vCameraList.back()->getTransform().AddLocalPosition(camDir.normalized() * DELTA_TIME);

	//m_vCameraList.back()->getTransform().SetPosition(m_pPlayer->getTransform().getPosition() + vector3::back() * 10.f);

	if (CInput::GetInstance().GetKeyDown(TWO))
		m_sOptions.lighting = !m_sOptions.lighting;

	m_pGraphicDev->AddRef();
	ULONG rcount = m_pGraphicDev->Release();
	CDebug::Log((int)rcount);
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

void CMainScene::Destroy()
{
#ifdef  _DEBUG
	CDebug::Release();
#endif
}
