// Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "CMainProcess.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#define MAX_LOADSTRING 100

// 전역 변수:                            
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(424);
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    CEngineEditor::GetInstance().MyRegisterClass(hInstance, WndProc);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
        return FALSE;

    CMainProcess::GetInstance().Ready_MainApp();

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;
    msg.message = WM_NULL;

    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            CMainProcess::GetInstance().Update_MainApp();
        }        
    }

    _ulong  dwRefCnt = 0;

    return (int) msg.wParam;
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   if (FAILED(CScreen::GetInstance().Start_Window(hInstance, nCmdShow)))
       return FALSE;

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    CEngineEditor::GetInstance().WndProcHandle(hWnd, message, wParam, lParam);

    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(CEngineEditor::GetInstance().getHInstance(), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        if (hWnd == CScreen::GetInstance().getWindowHandle(L"Base"))
        {
            HDC hdc = BeginPaint(hWnd, &ps);

            HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
            FillRect(hdc, &ps.rcPaint, blackBrush);
            DeleteObject(blackBrush);
        }
        else
        {
            HDC hdc = BeginPaint(hWnd, &ps);

            HBRUSH blackBrush = CreateSolidBrush(RGB(56, 56, 56));
            FillRect(hdc, &ps.rcPaint, blackBrush);
            DeleteObject(blackBrush);
        }

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_CTLCOLORSTATIC:
    {
        //HDC hdcStatic = (HDC)wParam;
        //HWND hStatic = (HWND)lParam;

        //if (hStatic == CEngineEditor::GetInstance().getMainTopBar() 
        //    || hStatic == CEngineEditor::GetInstance().getMainBottomBar())
        //{
        //    SetBkMode(hdcStatic, TRANSPARENT);
        //    SetBkColor(hdcStatic, RGB(0, 0, 0));
        //    static HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
        //    return (INT_PTR)hBlackBrush;
        //}
    }
    break;

    case WM_SIZE:
    {
        RECT rcClient;
        GetClientRect(hWnd, &rcClient);
        _int width = rcClient.right - rcClient.left;
        _int height = rcClient.bottom - rcClient.top;

        if (hWnd == CScreen::GetInstance().getWindowHandle(L"Scene"))
        {
            CMainProcess::GetInstance().OnSceneScreenChange(width, height);
        }
        else if (hWnd == CScreen::GetInstance().getWindowHandle(L"Game"))
        {
            CMainProcess::GetInstance().OnGameScreenChange(width, height);
        }
    }
    break;

    case WM_DESTROY:
        if (hWnd == CScreen::GetInstance().getWindowHandle(L"Base"))
            PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return TRUE;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
