#include "epch.h"
#include "MainProcess.h"
#include "TestClass.h"

CMainProcess::CMainProcess()
{
}

CMainProcess::~CMainProcess()
{
}

HRESULT CMainProcess::Initialize()
{
	CTestClass::GetInstance();

	if (FAILED(CDebug::GetInstance().Initialize()))
		return E_FAIL;
	if (FAILED(CTime::GetInstance().Initialize()))
		return E_FAIL;
	if (FAILED(CGraphicDevice::GetInstance().Initialize()))
		return E_FAIL;

	return S_OK;
}

void CMainProcess::Update_MainApp()
{
	CScene* scene = CSceneManager::GetInstance().Get_CrtScene();

	CTime::GetInstance().Update();
	CInput::GetInstance().Update();

	if (scene)
	{
		scene->Update();
		scene->LateUpdate();

		scene->Render_Game();
	}
}
