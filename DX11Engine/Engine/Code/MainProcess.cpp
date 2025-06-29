#include "epch.h"
#include "MainProcess.h"

CMainProcess::CMainProcess()
{
}

CMainProcess::~CMainProcess()
{
}

CMainProcess& CMainProcess::GetInstance()
{
	static CMainProcess inst;
	return inst;
}

HRESULT CMainProcess::Initialize()
{
    if (FAILED(CDebug::GetInstance().Initialize()))
        return E_FAIL;
    if (FAILED(CTime::GetInstance().Initialize()))
        return E_FAIL;
    if (FAILED(CInput::GetInstance().Initialize()))
        return E_FAIL;
    if (FAILED(CGraphicDevice::GetInstance().Initialize()))
        return E_FAIL;
    if (FAILED(CEditor::GetInstance().Initialize()))
        return E_FAIL;

#ifdef _DEBUG
    CGraphicDevice::GetInstance().Add_SwapChain(
        CEditor::GetInstance().Get_EditorWindow(),
        WINMODE::MODE_WINDOW,
        CDisplay::GetInstance().Get_ScreenResolution().x,
        CDisplay::GetInstance().Get_ScreenResolution().y
    );
#endif

    CGraphicDevice::GetInstance().Add_SwapChain(
        CDisplay::GetInstance().Get_GameWindow(),
        WINMODE::MODE_WINDOW,
        CDisplay::GetInstance().Get_ScreenResolution().x,
        CDisplay::GetInstance().Get_ScreenResolution().y
    );

    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return S_OK;
}

void CMainProcess::Update_MainApp()
{
    CScene* scene = CSceneManager::GetInstance().Get_CrtScene();

    CTime::GetInstance().Update();
    CInput::GetInstance().Update();

    CGraphicDevice& graphicDev = CGraphicDevice::GetInstance();

    if (scene)
    {
        scene->Update_Editor();
        scene->Update();
        scene->LateUpdate();

#ifdef _DEBUG
        graphicDev.Set_RenderTarget(CEditor::GetInstance().Get_EditorWindow());
        scene->Render_Editor();
        graphicDev.Present();
#endif

        graphicDev.Set_RenderTarget(CDisplay::GetInstance().Get_GameWindow());
        scene->Render_Game();
        graphicDev.Present();
    }
}
