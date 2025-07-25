#include "cpch.h"
#include "LoadingScene.h"

CLoadingScene::CLoadingScene()
	: m_pMainCamera(nullptr)
	, m_pCanvas(nullptr)
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
	bgObj->Get_Transform()->SetParent(m_pCanvas->Get_Transform());

	CGameObject* bottomObject = Add_GameObject(L"Bottom");
	CImage* bottomImage = bottomObject->AddComponent<CImage>();
	bottomImage->Get_Transform()->SetParent(m_pCanvas->Get_Transform());

	bottomImage->SetColor(ColorValue::black());

	return S_OK;
}

void CLoadingScene::Update()
{
	__super::Update();
}
