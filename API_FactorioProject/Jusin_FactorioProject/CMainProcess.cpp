#include "CMainProcess.h"
#include "CRandom.h"
#include "CSceneManager.h"
#include "CInput.h"
#include "CScreen.h"
#include "CResourceManager.h"
#include "CAudioManager.h"
#include "CMapManager.h"
#include "CControlManager.h"
#include "CPhysics.h"
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

extern HWND g_hWnd;
wstring g_strNextScene;
bool g_rectRencder;

int testCount = 0;

CMainProcess::CMainProcess()
    : m_dwTime(0)
    , m_hDC(0)
    , m_szFPS(L"")
    , m_fFixedCountTime((DWORD)GetTickCount64())
{
}

CMainProcess::~CMainProcess()
{
    Release();
}

void CMainProcess::Initialize()
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(71365);
#endif
    m_hDC = GetDC(g_hWnd);
    CRandom::GetInstance().Init();
    CAudioManager::GetInstance().Init();
    CResourceManager::GetInstance().Init();
    CPhysics::GetInstance().Init();
    CMapManager::GetInstance().Init();
    CItemManager::GetInstance().Init();
    CSceneManager::GetInstance().Init();
    CControlManager::GetInstance().Init();
    CSceneManager::GetInstance().ChangeScene(L"Main Scene");
}

void CMainProcess::MainLoop()
{
    if (g_strNextScene != L"")
    {
        CSceneManager::GetInstance().ChangeScene(g_strNextScene);
        g_strNextScene = L"";
    }

    CTime::GetInstance().Update();
    if (GetForegroundWindow() == g_hWnd)
        CInput::GetInstance().Update();
    if (CInput::GetInstance().GetKeyDown(KEY_CODE::ONE))
        CSceneManager::GetInstance().LoadScene(L"Test Scene");
    {
        wchar_t szMousePos[64];
        int xPos = CInput::GetInstance().GetMousePos().x;
        int yPos = CInput::GetInstance().GetMousePos().y;
        swprintf_s(szMousePos, L"Mouse Pos: (%d, %d)", xPos, yPos);
    }
    {
        wchar_t szCamPos[64];
        float xPos = CSceneManager::GetInstance().getCurrentscene()->getMainCamera()->GetCameraPosToWord().x;
        float yPos = CSceneManager::GetInstance().getCurrentscene()->getMainCamera()->GetCameraPosToWord().y;
        swprintf_s(szCamPos, L"Camera Pos: (%d, %d)", (int)xPos, (int)yPos);
    }

    CControlManager::GetInstance().Update();
    CPhysics::GetInstance().Update();
    CSceneManager::GetInstance().getCurrentscene()->Update();

    static uint64_t previousTime = GetTickCount64();
    static double accumulator = 0.0;
    uint64_t currentTime = GetTickCount64();

    double deltaTime = (currentTime - previousTime) * 0.001;
    previousTime = currentTime;
    accumulator += deltaTime;

    while (accumulator >= FIXED_TIMESTEP)
    {
        accumulator -= FIXED_TIMESTEP;
    }

    CPhysics::GetInstance().FixedUpdate();
    CSceneManager::GetInstance().getCurrentscene()->FixedUpdate();

    if (GetForegroundWindow() == g_hWnd)
        CInput::GetInstance().LateUpdate();
    else
        CInput::GetInstance().Reset();

    CSceneManager::GetInstance().getCurrentscene()->LateUpdate();
    CControlManager::GetInstance().LateUpdate();
}

void CMainProcess::MainDisplay()
{
    ++m_iFPS;

    if (m_dwTime + 1000 < GetTickCount64())
    {
        swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);
        SetWindowText(g_hWnd, m_szFPS);
        m_iFPS = 0;
        m_dwTime = (ULONG)GetTickCount64();
    }

    HDC hdc = GetDC(g_hWnd);
    HDC memDC = CreateCompatibleDC(hdc);
    RECT rc = { 0, 0, CScreen::GetInstance().getOptions().width, CScreen::GetInstance().getOptions().height };
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);
    HBRUSH hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
    FillRect(memDC, &rc, hBrush);

    CSceneManager::GetInstance().getCurrentscene()->Render(memDC);

    BitBlt(hdc, 0, 0, rc.right, rc.bottom, memDC, 0, 0, SRCCOPY);
    SelectObject(memDC, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(memDC);
    ReleaseDC(g_hWnd, hdc);
}

void CMainProcess::Release()
{
}
