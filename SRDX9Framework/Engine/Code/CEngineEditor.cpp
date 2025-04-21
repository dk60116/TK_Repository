#include "CEngineEditor.h"
#include "CScreen.h"

CEngineEditor::CEngineEditor()
	: m_hInst(nullptr)
	, m_hMainWnd(nullptr)
	, m_bPaused(false)
	, m_hTopBar(nullptr)
	, m_hTop_Scene(nullptr)
	, m_hTop_Game(nullptr)
	, m_hBtnPause(nullptr)
	, m_hBtnStop(nullptr)
	, m_hBtnNextFrame(nullptr)
{
}

CEngineEditor::~CEngineEditor()
{
}

void CEngineEditor::Init_Main(HINSTANCE _hInst, HWND _mainWnd)
{
	m_hMainWnd = _mainWnd;
	m_hInst = _hInst;

	RECT windowRect;
	GetWindowRect(m_hMainWnd, &windowRect);

	int screenX = windowRect.right + windowRect.left;
	int screenXCenter = screenX / 2;

	int buttonsWidth = 36;
	int buttonsHeight = 24;
	int buttonsYPos = 8;
	int spacing = 18;

	// 상단바 영역
	m_hTopBar = CreateWindowW(L"STATIC", nullptr,
		WS_VISIBLE | WS_CHILD,
		0, 0, screenX, MAINTOPBARHEIGHT,
		m_hMainWnd, nullptr, m_hInst, nullptr);

	// 종료 버튼
	m_hBtnStop = CreateWindowW(L"BUTTON", L"■", 
		WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
		screenXCenter - buttonsWidth - spacing, buttonsYPos,
		buttonsWidth, buttonsHeight,
		m_hMainWnd, (HMENU)ID_BTN_STOP, m_hInst, nullptr);

	// 일시정지 / 재생 버튼
	m_hBtnPause = CreateWindowW(L"BUTTON", L"II", 
		WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
		screenXCenter - buttonsWidth / 2, buttonsYPos,
		buttonsWidth, buttonsHeight,
		m_hMainWnd, (HMENU)ID_BTN_PAUSE, m_hInst, nullptr);

	// 다음 프레임 버튼
	m_hBtnNextFrame = CreateWindowW(L"BUTTON", L"▶I", 
		WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
		screenXCenter + spacing, buttonsYPos,
		buttonsWidth, buttonsHeight,
		m_hMainWnd, (HMENU)ID_BTN_NEXTFRAME, m_hInst, nullptr);

	RECT mainRC;
	GetClientRect(m_hMainWnd, &mainRC);

	_int bottomY = mainRC.bottom;

	// 하단바 영역
	m_hBottomBar = CreateWindowW
	(
		L"STATIC", nullptr,
		WS_VISIBLE | WS_CHILD,
		0, 
		bottomY - CHILDTOPBARHEIGHT,
		screenX,
		bottomY,
		m_hMainWnd, nullptr, m_hInst, nullptr
	);

	HFONT hFont14 = CreateDefaultFont(L"Arial", 18);
	HFONT hFont16 = CreateDefaultFont(L"Arial", 16, TRUE);

	SendMessageW(m_hBtnStop, WM_SETFONT, (WPARAM)hFont14, TRUE);
	SendMessageW(m_hBtnPause, WM_SETFONT, (WPARAM)hFont16, TRUE);
	SendMessageW(m_hBtnNextFrame, WM_SETFONT, (WPARAM)hFont14, TRUE);
	
	EnableWindow(m_hBtnStop, TRUE);
	EnableWindow(m_hBtnPause, TRUE);
	EnableWindow(m_hBtnNextFrame, m_bPaused);
}

void CEngineEditor::Init_Scene(HWND _gameWnd)
{
	RECT windowRect;
	GetWindowRect(_gameWnd, &windowRect);

	int screenX = windowRect.right - windowRect.left;
	int screenXCenter = screenX / 2;

	// 상단바 영역
	m_hTop_Scene = CreateWindowW(L"STATIC", nullptr,
		WS_VISIBLE | WS_CHILD,
		0, 0, screenX, CHILDTOPBARHEIGHT,
		_gameWnd, nullptr, m_hInst, nullptr);

	HFONT hFont12 = CreateFontW
	(
		14, 0, 0, 0,
		FW_NORMAL, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_PITCH,
		DEFAULT_PITCH | FF_SWISS, L"Arial"
	);

	HWND ee = CreateWindowW(L"COMBOBOX", L"Handle", 
		WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
		10, 3, 100, 200,
		_gameWnd, NULL, m_hInst, nullptr);

	HFONT hFont16 = CreateDefaultFont(L"Arial", 16);
	SendMessageW(ee, WM_SETFONT, (WPARAM)hFont16, TRUE);

	SendMessageW(ee, CB_ADDSTRING, 0, (LPARAM)L"Center");
	SendMessageW(ee, CB_ADDSTRING, 1, (LPARAM)L"Pivot");

	SendMessageW(ee, CB_SETCURSEL, 0, 0);
}

void CEngineEditor::Init_Game(HWND _sceneWnd)
{
	RECT windowRect;
	GetWindowRect(_sceneWnd, &windowRect);

	int screenX = windowRect.right - windowRect.left;
	int screenXCenter = screenX / 2;

	// 상단바 영역
	m_hTop_Game = CreateWindowW(L"STATIC", nullptr,
		WS_VISIBLE | WS_CHILD,
		0, 0, screenX, 30,
		_sceneWnd, nullptr, m_hInst, nullptr);
}

LRESULT CEngineEditor::WndProcHandle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DRAWITEM:
		{
			LPDRAWITEMSTRUCT lpDraw = (LPDRAWITEMSTRUCT)lParam;

			if (lpDraw->CtlID == ID_BTN_STOP || lpDraw->CtlID == ID_BTN_PAUSE || lpDraw->CtlID == ID_BTN_NEXTFRAME)
			{
				HDC hdc = lpDraw->hDC;
				RECT rc = lpDraw->rcItem;

				COLORREF bgColor = RGB(0, 0, 0);
				COLORREF btnColor = (lpDraw->itemState & ODS_SELECTED) ? RGB(60, 60, 60) : RGB(80, 80, 80);
				HBRUSH hBGBrush = CreateSolidBrush(bgColor);
				HBRUSH hBrush = CreateSolidBrush(btnColor);

				// 둥근 사각형
				int cornerRadius = 8;
				HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				HGDIOBJ oldPen = SelectObject(hdc, hPen);
				HGDIOBJ oldBrush = SelectObject(hdc, hBrush);

				FillRect(hdc, &rc, hBGBrush);
				RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, cornerRadius, cornerRadius);

				// 텍스트
				SetBkMode(hdc, TRANSPARENT);
				SetTextColor(hdc, RGB(255, 255, 255));

				LPCWSTR text = L"";
				if (lpDraw->CtlID == ID_BTN_STOP)
					text = L"■";
				else if (lpDraw->CtlID == ID_BTN_PAUSE)
					text = L"❚❚";
				else if (lpDraw->CtlID == ID_BTN_NEXTFRAME)
					text = L"▶❚";

				if (lpDraw->CtlID == ID_BTN_STOP)
				{
					RECT textRect = rc;
					OffsetRect(&textRect, 0, 0);

					HFONT hFont = CreateDefaultFont(L"Segoe UI Variable", 14);

					HGDIOBJ oldFont = SelectObject(hdc, hFont);

					DrawText(hdc, text, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

					SelectObject(hdc, oldFont);
					DeleteObject(hFont);
				}
				else if (lpDraw->CtlID == ID_BTN_PAUSE)
				{
					RECT textRect = rc;
					OffsetRect(&textRect, 0, -1);

					HFONT hFont = CreateDefaultFont(L"Segoe UI Variable", 20);

					HGDIOBJ oldFont = SelectObject(hdc, hFont);

					DrawText(hdc, text, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

					SelectObject(hdc, oldFont);
					DeleteObject(hFont);
				}
				else if (lpDraw->CtlID == ID_BTN_NEXTFRAME)
				{
					RECT textRect = rc;
					OffsetRect(&textRect, 0, 0);

					HFONT hFont = CreateDefaultFont(L"Arial", 16);

					HGDIOBJ oldFont = SelectObject(hdc, hFont);

					DrawText(hdc, text, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

					SelectObject(hdc, oldFont);
					DeleteObject(hFont);
				}

				// 정리
				SelectObject(hdc, oldPen);
				SelectObject(hdc, oldBrush);
				DeleteObject(hBGBrush);
				DeleteObject(hBrush);
				DeleteObject(hPen);

				return TRUE;
			}
		}
	}

	return 0;
}

CEngineEditor::GameRunningState CEngineEditor::HandleCommand(WPARAM _wParam)
{
	switch (LOWORD(_wParam))
	{
	case ID_BTN_PAUSE:
		m_bPaused = !m_bPaused;
		SetWindowText(m_hBtnPause, m_bPaused ? L"▶" : L"II");

		EnableWindow(m_hBtnNextFrame, m_bPaused);

		return m_bPaused ? PAUSED : RUNNING;

	case ID_BTN_STOP:
		PostQuitMessage(0);
		return STOPPED;

	case ID_BTN_NEXTFRAME:
		SetWindowText(m_hBtnPause, L"▶");
		return PAUSED;

	default:
		return STOPPED;
	}

	return STOPPED;
}

void CEngineEditor::UpdateResolution(const vector2Int _resolution)
{
	int screenX = _resolution.x;
	int screenXCenter = screenX / 2;

	MoveWindow(m_hTopBar,
		0, 0,
		screenX, 30,
		true);

	MoveWindow(m_hBtnStop,
		screenXCenter - 40 - 20, 1,
		40, 28,
		true);

	MoveWindow(m_hBtnPause,
		screenXCenter - 20, 1,
		40, 28,
		true);

	MoveWindow(m_hBtnNextFrame,
		screenXCenter + 20, 1,
		40, 28,
		true);
}

HFONT CEngineEditor::CreateDefaultFont(LPCWSTR _font, _float _size, _bool _bold)
{
	HFONT result = CreateFontW
	(
		_size, 0, 0, 0,
		_bold ? FW_BOLD : FW_NORMAL, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_PITCH,
		DEFAULT_PITCH | FF_SWISS, _font
	);

	return result;
}
