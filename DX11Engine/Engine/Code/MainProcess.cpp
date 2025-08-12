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
    if (FAILED(CTime::Initialize()))
        return E_FAIL;
    if (FAILED(CDebug::Initialize()))
        return E_FAIL;
    if (FAILED(CGraphicDevice::Initialize()))
        return E_FAIL;
    if (FAILED(CRandom::Initialize()))
        return E_FAIL;
    if (FAILED(CEditor::Initialize()))
        return E_FAIL;
    if (FAILED(CResources::Initialize()))
        return E_FAIL;
    if (FAILED(CSceneManager::Initialize()))
        return E_FAIL;
    if (FAILED(CSceneLoader::Initialize()))
        return E_FAIL;
    if (FAILED(CInput::Initialize()))
        return E_FAIL;
    if (FAILED(CUIManager::Initialize()))
        return E_FAIL;
    if (FAILED(CCollisionManager::Initialize()))
        return E_FAIL;

#ifndef _CLIENT_BUILD
    CEditor::EDITORWINOPTION sOption = CEditor::Get_Options();

    vector2Int offsetMin = vector2Int(0, (_int)sOption.topBarHeight);
    vector2Int offsetMax = vector2Int(_int(sOption.projectWidth + sOption.hierachyWidth + sOption.inspectorWidth), 0);

    CGraphicDevice::Add_SwapChain
    (
        CEditor::Get_EditorWindow(),
        WINMODE::MODE_WINDOW,
        CEditor::Get_WindowResolution().x,
        CEditor::Get_WindowResolution().y,
        offsetMin,
        offsetMax
    );
#endif

    CGraphicDevice::Add_SwapChain
    (
        CDisplay::Get_GameWindow(),
        WINMODE::MODE_WINDOW,
        CDisplay::Get_ScreenResolution().x,
        CDisplay::Get_ScreenResolution().y
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
    CScene* scene = CSceneManager::Get_CrtScene();

    CTime::Update();
    CInput::Update();

    if (scene)
    {
        scene->Update_Editor();
        scene->Update();
        scene->LateUpdate();
        scene->FixedUpdate();

#ifndef _CLIENT_BUILD
        CGraphicDevice::Set_RenderTarget(CEditor::Get_EditorWindow());

        CEditor::Editor_Update_Begin();
        CEditor::Editor_Update_During();
        scene->Render_Editor();
        CEditor::Editor_Update_End();

        CGraphicDevice::Present();
#endif

        CGraphicDevice::Set_RenderTarget(CDisplay::Get_GameWindow());
        scene->Render_Game();
        CGraphicDevice::Present();
    }

    if (CSceneManager::Is_Loading() && !CSceneLoader::Is_Loading())
        CSceneManager::LoadComplete();
}

void CMainProcess::Release_MainApp()
{
    CDebug::Release();
}