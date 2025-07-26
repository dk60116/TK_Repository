#include "cpch.h"
#include "LoadingScene.h"

CLoadingScene::CLoadingScene()
	: m_pMainCamera(nullptr)
	, m_pCanvas(nullptr)
	, m_pLogoImage(nullptr)
{
}

CLoadingScene::~CLoadingScene()
{
}

HRESULT CLoadingScene::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	CGameObject* cameraObject = Add_GameObject(L"Main Camera");
	m_pMainCamera = cameraObject->AddComponent<CCamera>();

	CGameObject* canvasObj = Add_GameObject(L"Canvas");
	m_pCanvas = canvasObj->AddComponent<CCanvas>();

	CGameObject* bgObj = Add_GameObject(L"BG");
	CImage* bgImage = bgObj->AddComponent<CImage>();
	bgImage->Get_Transform()->SetParent(m_pCanvas->Get_Transform());

	CGameObject* bottomObject = Add_GameObject(L"Bottom");
	CImage* bottomImage = bottomObject->AddComponent<CImage>();
	bottomImage->Get_Transform()->SetParent(m_pCanvas->Get_Transform());
	bottomImage->SetColor(ColorValue::black());

	CGameObject* backLogoObj = Add_GameObject(L"Logo_Back");
	CImage* backLogoImage = backLogoObj->AddComponent<CImage>();
	backLogoImage->Get_Transform()->SetParent(m_pCanvas->Get_Transform());
	backLogoImage->SetTexture(CResources::GetInstance().LoadOnScene<CTexture>(L"Logo (Texture)"));
	backLogoImage->SetColor(ColorValue::gray(0.5f));

	CGameObject* logoObj = Add_GameObject(L"Logo");
	m_pLogoImage = logoObj->AddComponent<CImage>();
	m_pLogoImage->Get_Transform()->SetParent(backLogoImage->Get_Transform());
	m_pLogoImage->SetTexture(CResources::GetInstance().LoadOnScene<CTexture>(L"Logo (Texture)"));

	return S_OK;
}

void CLoadingScene::Update()
{
	__super::Update();

	if (CInput::GetInstance().GetKey_Editor(M))
	{
		m_pLogoImage->Get_Transform()->Set_LocalEulerAnglesZ(40.f);
	}
}
