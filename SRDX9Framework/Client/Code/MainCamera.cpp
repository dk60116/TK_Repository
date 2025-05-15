#include "pch.h"
#include "MainCamera.h"

CMainCamera::CMainCamera()
	: m_pCamera(nullptr)
{
	m_strName = L"MainCamera";
}

CMainCamera::~CMainCamera()
{
}

CMainCamera* CMainCamera::Create()
{
	return new CMainCamera();
}

void CMainCamera::Init()
{
	if (!m_pCamera)
		m_pCamera = m_pGameObject->AddComponent<CCamera>();

	m_pCamera->SetMode(CCamera::ORTHOGRAPHIC);
	m_pCamera->SetSize(11);
}

void CMainCamera::Awake()
{
}

void CMainCamera::Start()
{
}

void CMainCamera::Update()
{
}

void CMainCamera::FixedUpdate()
{
}

void CMainCamera::LateUpdate()
{
}

void CMainCamera::Render()
{
}

void CMainCamera::OnEnable()
{
}

void CMainCamera::OnDisable()
{
}

void CMainCamera::OnDestroy()
{
	__super::OnDestroy();
}
