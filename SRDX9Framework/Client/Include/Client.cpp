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
    //_CrtSetBreakAlloc(1755);
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
    case WM_CTLCOLOREDIT:
    {
        HWND hEdit = (HWND)lParam;
            HDC hdc = (HDC)wParam;
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(255, 255, 255));
            SetBkColor(hdc, RGB(30, 30, 30));

            static HBRUSH hBrush = CreateSolidBrush(RGB(30, 30, 30));
            return (LRESULT)hBrush;
        break;
    }
    break;
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
        if (hWnd == CEngineEditor::GetInstance().FindWindowHandle(L"Base"))
        {
            HDC hdc = BeginPaint(hWnd, &ps);

            HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
            FillRect(hdc, &ps.rcPaint, blackBrush);
            DeleteObject(blackBrush);
        }
        else
        {
            HDC hdc = BeginPaint(hWnd, &ps);

            HBRUSH blackBrush = CreateSolidBrush(CEngineEditor::GetInstance().getOptions().baseColor.rColor());
            FillRect(hdc, &ps.rcPaint, blackBrush);
            DeleteObject(blackBrush);
        }

        if (hWnd == CEngineEditor::GetInstance().FindWindowHandle(L"Hierachy Side"))
        {
            HDC hdc = BeginPaint(hWnd, &ps);

            HBRUSH blackBrush = CreateSolidBrush(ColorValue(25, 25, 25).rColor());
            FillRect(hdc, &ps.rcPaint, blackBrush);
            DeleteObject(blackBrush);
        }

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        HWND hStatic = (HWND)lParam;

        wchar_t windowTitle[256] = { 0 };
        GetWindowTextW(hStatic, windowTitle, sizeof(windowTitle) / sizeof(wchar_t));

        if (GetWindowLongPtr(hStatic, GWLP_USERDATA) == HWND_CHILDTOPBAR)
        {
            SetBkMode(hdcStatic, TRANSPARENT);
            SetBkColor(hdcStatic, ColorValue::black().rColor());
            static HBRUSH hBlackBrush = CreateSolidBrush(CEngineEditor::GetInstance().getOptions().baseColor.rColor());
            return (INT_PTR)hBlackBrush;
        }
        else if (GetWindowLongPtr(hStatic, GWLP_USERDATA) == HWND_BASETOPBAR || GetWindowLongPtr(hStatic, GWLP_USERDATA) == HWND_BASEBOTTOMBAR)
        {
            SetBkMode(hdcStatic, TRANSPARENT);
            SetBkColor(hdcStatic, ColorValue::black().rColor());
            static HBRUSH hBlackBrush = CreateSolidBrush(ColorValue::white().black());
            return (INT_PTR)hBlackBrush;
        }
        else if (GetWindowLongPtr(hStatic, GWLP_USERDATA) == HWND_HIERACHYSIDE)
        {
            SetBkMode(hdcStatic, TRANSPARENT);
            SetBkColor(hdcStatic, ColorValue::white().black());
            static HBRUSH hBlackBrush = CreateSolidBrush(ColorValue(49, 49, 49).rColor());
            return (INT_PTR)hBlackBrush;
        }
        else if (GetWindowLongPtr(hStatic, GWLP_USERDATA) == HWND_INPUTBOX)
        {
            SetBkMode(hdcStatic, TRANSPARENT);
            SetBkColor(hdcStatic, ColorValue::white().black());
            static HBRUSH hBlackBrush = CreateSolidBrush(CEngineEditor::GetInstance().getOptions().baseColor.rColor());
            return (INT_PTR)hBlackBrush;
        }
        else if (GetWindowLongPtr(hStatic, GWLP_USERDATA) == HWND_INSPECTORCOMPONENTTOP)
        {
            SetBkMode(hdcStatic, TRANSPARENT);
            SetBkColor(hdcStatic, ColorValue::white().black());
            SetTextColor(hdcStatic, CEngineEditor::GetInstance().getOptions().baseTextColor.rColor());  
            static HBRUSH hBlackBrush = CreateSolidBrush(ColorValue(62, 62, 62).rColor());
            return (INT_PTR)hBlackBrush;
        }
    }
    break;

    case WM_LBUTTONDOWN:
    {
    }
    break;
    
    case WM_LBUTTONUP:
    {
    }
    break;

    case WM_NOTIFY:
    {
        HWND treeViewHnd = CEngineEditor::GetInstance().getWindow<CHierachyWindow>()->getTreeHandle();

        LPNMHDR pNMHDR = reinterpret_cast<LPNMHDR>(lParam);

        if (pNMHDR->code == TVN_SELCHANGED)
        {
            LPNMTREEVIEW pNMTV = reinterpret_cast<LPNMTREEVIEW>(lParam);

            HTREEITEM hSelectedItem = pNMTV->itemNew.hItem;
            TVITEMW tvi = {};
            tvi.hItem = hSelectedItem;
            tvi.mask = TVIF_PARAM;
        }

        LPNMHDR phdr = (LPNMHDR)lParam;

        // 트리뷰 커스텀-드로잉만 처리
        wchar_t buf[64] = {};
        GetWindowTextW(pNMHDR->hwndFrom, buf, 64);

        if (pNMHDR->code == NM_DBLCLK)
        {
            return TRUE;
        }

        if (pNMHDR->code == NM_CUSTOMDRAW &&
            wcscmp(buf, L"Hierachy Tree") == 0)
        {
            auto pCD = reinterpret_cast<LPNMTVCUSTOMDRAW>(lParam);

            _bool hot = pCD->nmcd.uItemState & CDIS_HOT;
            HWND hwndTree = pNMHDR->hwndFrom;
            _bool selected = pCD->nmcd.uItemState & CDIS_SELECTED;
            _bool mouseDown = (GetKeyState(VK_LBUTTON) & 0x8000) != 0;
            HTREEITEM hItem = (HTREEITEM)pCD->nmcd.dwItemSpec;
            RECT rcItem;

            TreeView_GetItemRect(hwndTree, hItem, &rcItem, TRUE);

            HTREEITEM hSelected = (HTREEITEM)SendMessage(hwndTree, TVM_GETNEXTITEM, TVGN_CARET, 0);

            HTREEITEM hParent = TreeView_GetParent(hwndTree, hItem);

            _bool isRoot = (hParent == NULL);

            switch (pCD->nmcd.dwDrawStage)
            {
            case CDDS_PREPAINT:
                return CDRF_NOTIFYITEMDRAW;

            case CDDS_ITEMPREPAINT:
            {
                if (pNMHDR->code == NM_CUSTOMDRAW)
                {
                    LPNMTVCUSTOMDRAW pCustomDraw = reinterpret_cast<LPNMTVCUSTOMDRAW>(lParam);

                    pCustomDraw->clrText = RGB(255, 255, 255); // 흰 글자

                    if (hot)  // 마우스 오버 상태
                    {
                        ColorValue bgColor = mouseDown ?
                             ColorValue(44, 93, 135) : ColorValue(68, 68, 68);

                        pCustomDraw->clrTextBk = bgColor.rColor(); // 배경
                    }

                    if (selected && !mouseDown)
                    {
                        if (GetForegroundWindow() == CEngineEditor::GetInstance().FindWindowHandle(L"Hierachy"))
                            pCustomDraw->clrTextBk = RGB(44, 93, 135); // 푸른 배경
                        else
                            pCustomDraw->clrTextBk = RGB(77, 77, 77); // 밝은 회색 배경
                    }

                    if (isRoot && !selected)
                    {
                        pCustomDraw->clrTextBk = RGB(35, 35, 35); // 짙은 배경
                    }

                    HFONT hDef = (HFONT)SendMessage(pNMHDR->hwndFrom, WM_GETFONT, 0, 0);
                    LOGFONTW lf;  GetObjectW(hDef, sizeof(lf), &lf);
                    lf.lfUnderline = FALSE;
                    HFONT hNoUL = CreateFontIndirectW(&lf);
                    SelectObject(pCD->nmcd.hdc, hNoUL);
                }
            }
            return CDRF_NEWFONT | CDRF_NOTIFYPOSTPAINT;

            case CDDS_ITEMPOSTPAINT:
            {
                HWND tv = CEngineEditor::GetInstance()
                    .getWindow<CHierachyWindow>()
                    ->getTreeHandle();
                HTREEITEM hItem = (HTREEITEM)pCD->nmcd.dwItemSpec;

                TVITEMEXW info{};
                info.hItem = hItem;
                info.mask = TVIF_STATE | TVIF_CHILDREN;
                info.stateMask = TVIS_EXPANDED;
                TreeView_GetItem(tv, &info);

                if (info.cChildren > 0)
                {
                    RECT rcText;  TreeView_GetItemRect(tv, hItem, &rcText, TRUE);
                    RECT rcBtn = rcText;
                    const int w = 10;
                    rcBtn.right = rcText.left - 4;
                    rcBtn.left = rcBtn.right - w;
                    rcBtn.top += (rcText.bottom - rcText.top - w) / 2;
                    rcBtn.bottom = rcBtn.top + w + 1;

                    COLORREF btnBGColor = 
                        (info.state & TVIS_SELECTED)
                        ? RGB(44, 93, 135) : CEngineEditor::GetInstance().getOptions().baseColor.rColor();

                    if (hot)
                    {
                        btnBGColor = RGB(68, 68, 68);

                        if (selected)
                            btnBGColor = RGB(44, 93, 135);
                        else if (mouseDown)
                            btnBGColor = RGB(44, 93, 135);
                    }

                    if (mouseDown && !hot)
                    {
                        btnBGColor = CEngineEditor::GetInstance().getOptions().baseColor.rColor();
                    }

                    if (isRoot && !selected)
                        btnBGColor = RGB(35, 35, 35);

                    if (selected && GetForegroundWindow() != CEngineEditor::GetInstance().FindWindowHandle(L"Hierachy"))
                        btnBGColor = RGB(77, 77, 77); // 밝은 회색 배경

                    // 기존 +/– 덮기
                    HBRUSH hBrush = CreateSolidBrush(btnBGColor);
                    FillRect(pCD->nmcd.hdc, &rcBtn, hBrush);
                    DeleteObject(hBrush);

                    // ▶ / ▼ 출력
                    const wchar_t* glyph = (info.state & TVIS_EXPANDED) ? L"▼" : L"▶";

                    HFONT hFontDefault = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
                    HFONT hFontBig = NULL;

                    if (wcscmp(glyph, L"▼") == 0)
                    {
                        LOGFONT lf{};
                        GetObject(hFontDefault, sizeof(LOGFONT), &lf);
                        lf.lfWidth = static_cast<LONG>(-6);
                        lf.lfHeight = static_cast<LONG>(-12);
                        hFontBig = CreateFontIndirect(&lf);
                    }
                    else if (wcscmp(glyph, L"▶") == 0)
                    {
                        LOGFONT lf{};
                        GetObject(hFontDefault, sizeof(LOGFONT), &lf);
                        lf.lfWidth = static_cast<LONG>(-7);
                        lf.lfHeight = static_cast<LONG>(-13);
                        hFontBig = CreateFontIndirect(&lf);
                    }

                    HFONT hOldFont = (HFONT)SelectObject(pCD->nmcd.hdc,
                        (hFontBig) ? hFontBig : hFontDefault);

                    SetBkMode(pCD->nmcd.hdc, TRANSPARENT);
                    SetTextColor(pCD->nmcd.hdc, ColorValue(104, 104, 104).rColor());
                    DrawTextW(pCD->nmcd.hdc, glyph, 1, &rcBtn,
                        DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                    SelectObject(pCD->nmcd.hdc, hOldFont);
                    if (hFontBig) DeleteObject(hFontBig);

                    CEngineEditor::GetInstance().getWindow<CHierachyWindow>()->AdjustTreeHeight();
                }
            }
                return CDRF_SKIPDEFAULT;
            } 
        }
        return CDRF_DODEFAULT;
    }
    break;

    case WM_SIZE:
    {
        RECT rcClient;
        GetClientRect(hWnd, &rcClient);
        _int width = rcClient.right - rcClient.left;
        _int height = rcClient.bottom - rcClient.top;

        if (hWnd == CEngineEditor::GetInstance().FindWindowHandle(L"Scene"))
            CMainProcess::GetInstance().OnSceneScreenChange(width, height);
        else if (hWnd == CEngineEditor::GetInstance().FindWindowHandle(L"Game"))
            CMainProcess::GetInstance().OnGameScreenChange(width, height);
    }
    break;

    case WM_DESTROY:
        if (hWnd == CEngineEditor::GetInstance().FindWindowHandle(L"Base"))
            PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
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
