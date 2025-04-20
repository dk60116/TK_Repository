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

	int screenX = windowRect.right - windowRect.left;
	int screenXCenter = screenX / 2;

	int buttonsWidth = 34;
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

	// 일시정지/재생 버튼
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
		0, 0, screenX, 30,
		_gameWnd, nullptr, m_hInst, nullptr);

	HFONT hFont = CreateFontW
	(
		16, 0, 0, 0, 
		FW_NORMAL, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Areal"
	);

	HWND ee = CreateWindowW(L"COMBOBOX", L"Handle", 
		WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
		10, 3, 100, 200,
		_gameWnd, NULL, m_hInst, nullptr);

	SendMessageW(ee, WM_SETFONT, (WPARAM)hFont, TRUE);

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

			// 커스터마이징할 버튼들만 필터링
			if (lpDraw->CtlID == ID_BTN_STOP || lpDraw->CtlID == ID_BTN_PAUSE || lpDraw->CtlID == ID_BTN_NEXTFRAME)
			{
				HDC hdc = lpDraw->hDC;
				RECT rc = lpDraw->rcItem;

				// 배경색 (선택적으로 눌림 여부에 따라 색 다르게)
				COLORREF bgColor = (lpDraw->itemState & ODS_SELECTED) ? RGB(60, 60, 60) : RGB(80, 80, 80);
				HBRUSH hBrush = CreateSolidBrush(bgColor);

				// 둥근 사각형 (rx, ry = 반지름)
				int cornerRadius = 8;
				HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				HGDIOBJ oldPen = SelectObject(hdc, hPen);
				HGDIOBJ oldBrush = SelectObject(hdc, hBrush);

				RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, cornerRadius, cornerRadius);

				// 텍스트
				SetBkMode(hdc, TRANSPARENT);
				SetTextColor(hdc, RGB(255, 255, 255));

				LPCWSTR text = L"";
				if (lpDraw->CtlID == ID_BTN_STOP)
					text = L"■";
				else if (lpDraw->CtlID == ID_BTN_PAUSE)
					text = L"II";
				else if (lpDraw->CtlID == ID_BTN_NEXTFRAME)
					text = L"▶I";

				DrawText(hdc, text, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				// 정리
				SelectObject(hdc, oldPen);
				SelectObject(hdc, oldBrush);
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
