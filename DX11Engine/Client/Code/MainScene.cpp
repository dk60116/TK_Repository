#include "cpch.h"
#include "MainScene.h"

CMainScene::CMainScene()
	: CScene{}
{
}

CMainScene::~CMainScene()
{
}

HRESULT CMainScene::Initialize()
{
	Add_GameObject(L"Mario");
	CGameObject* cameraObject = Add_GameObject(L"Main Camera");
	CCamera* camera = cameraObject->AddComponent<CCamera>();

	return S_OK;
}
