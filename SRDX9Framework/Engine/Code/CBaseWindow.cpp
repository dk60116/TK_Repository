#include "CBaseWindow.h"
#include "Screen.h"
#include "Management.h"

CBaseWindow::CBaseWindow()
	: m_hBottomBar(nullptr)
	, m_hPlayBtn(nullptr)
	, m_hPauseBtn(nullptr)
	, m_hOneFrameBtn(nullptr)
{
	m_strName = L"Editor:Base Window";
}

CBaseWindow::~CBaseWindow()
{
}

HRESULT CBaseWindow::Init(HWND _hWnd, vector2Int _size, _bool _isBase)
{
	m_iTopBarHeight = MAINTOPBARHEIGHT;
	m_bHaveWindowBar = true;
	
	if (FAILED(__super::Init(_hWnd, _size, true)))
		return E_FAIL;

	HINSTANCE hInstance = CEngineEditor::GetInstance().getHInstance();

	_int bottomY = m_sClinentRect.bottom;

	// 하단바
	m_hBottomBar = CreateWindowW
	(
		L"STATIC", nullptr,
		WS_VISIBLE | WS_CHILD,
		0, bottomY - CHILDTOPBARHEIGHT,
		_size.x,
		bottomY,
		_hWnd, nullptr, hInstance, nullptr
	);

	SetWindowLongPtr(m_hBottomBar, GWLP_USERDATA, static_cast<LONG_PTR>(HWND_BASEBOTTOMBAR));

	if (!m_hBottomBar)
		return E_FAIL;

	// 버튼
	int screenX = m_sWindowRect.right + m_sWindowRect.left;
	int screenXCenter = screenX / 2;

	int buttonsWidth = 36;
	int buttonsHeight = 22;
	int buttonsYPos = 8;
	int spacing = 18;

	// 플레이 버튼
	m_hPlayBtn = CreateWindowW(L"BUTTON", L"▶",
		WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
		screenXCenter - buttonsWidth - spacing, buttonsYPos,
		buttonsWidth, buttonsHeight,
		m_hWnd, (HMENU)ID_BTN_PLAY, hInstance, nullptr);

	// 일시정지 / 재생 버튼
	m_hPauseBtn = CreateWindowW(L"BUTTON", L"II",
		WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
		screenXCenter - buttonsWidth / 2, buttonsYPos,
		buttonsWidth, buttonsHeight,
		m_hWnd, (HMENU)ID_BTN_PAUSE, hInstance, nullptr);

	// 다음 프레임 버튼
	m_hOneFrameBtn = CreateWindowW(L"BUTTON", L"▶I",
		WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
		screenXCenter + spacing, buttonsYPos,
		buttonsWidth, buttonsHeight,
		m_hWnd, (HMENU)ID_BTN_NEXTFRAME, hInstance, nullptr);

	if (!m_hPlayBtn || !m_hPauseBtn || !m_hOneFrameBtn)
		return E_FAIL;

	EnableWindow(m_hPlayBtn, TRUE);
	EnableWindow(m_hPauseBtn, CEngineEditor::GetInstance().isPaused());
	EnableWindow(m_hOneFrameBtn, CEngineEditor::GetInstance().isPaused());

	return S_OK;
}

void CBaseWindow::Render()
{
}

void CBaseWindow::Update()
{
}

void CBaseWindow::UpdateResolution(HWND _target, vector2Int _resolution)
{
	if (_target != m_hWnd)
		return;

	__super::UpdateResolution(_target, _resolution);

	RECT mainRC;
	GetClientRect(m_hWnd, &mainRC);

	MoveWindow(m_hBottomBar, 0, m_sClinentRect.bottom - CHILDTOPBARHEIGHT, _resolution.x, CHILDTOPBARHEIGHT, TRUE);
	
	int screenX = _resolution.x;
	int screenXCenter = screenX / 2;
	int screenY = 8;

	int btnWidth = 40;
	int btnHeight = 26;

	MoveWindow(m_hPlayBtn,
	screenXCenter - btnWidth - btnWidth / 2, screenY,
	btnWidth, btnHeight,
	true);

	MoveWindow(m_hPauseBtn,
	screenXCenter - btnWidth / 2, screenY,
	btnWidth, btnHeight,
	true);

	MoveWindow(m_hOneFrameBtn,
	screenXCenter + btnWidth / 2, screenY,
	btnWidth, btnHeight,
	true);
}

LRESULT CBaseWindow::WndProcHandle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		switch (wmId)
		{
		case ID_BTN_PLAY:
			CEngineEditor::GetInstance().SwitchPlaying();
			EnableWindow(m_hPauseBtn, CEngineEditor::GetInstance().isPlaying());

			if (!CEngineEditor::GetInstance().isPlaying())
			{
				CEngineEditor::GetInstance().SetPaused(false);

				CManagement::GetInstance().LoadScene(CManagement::GetInstance().getCrtScene()->getName());
			}

			InvalidateRect(m_hPlayBtn, nullptr, TRUE);
			UpdateWindow(m_hPlayBtn);
			InvalidateRect(m_hPauseBtn, nullptr, TRUE);
			UpdateWindow(m_hPauseBtn);
			break;
		case ID_BTN_PAUSE:
			CEngineEditor::GetInstance().SwitchPaused();
			EnableWindow(m_hOneFrameBtn, CEngineEditor::GetInstance().isPaused());

			InvalidateRect(m_hPauseBtn, nullptr, TRUE);
			UpdateWindow(m_hPauseBtn);
			break;
		case ID_BTN_NEXTFRAME:
			CEngineEditor::GetInstance().SetNextFrame(true);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT lpDraw = (LPDRAWITEMSTRUCT)lParam;

		if (lpDraw->CtlID == ID_BTN_PLAY || lpDraw->CtlID == ID_BTN_PAUSE || lpDraw->CtlID == ID_BTN_NEXTFRAME)
		{
			HDC hdc = lpDraw->hDC;
			RECT rc = lpDraw->rcItem;

			COLORREF bgColor = RGB(0, 0, 0);
			COLORREF btnColor = (lpDraw->itemState & ODS_SELECTED) ? RGB(60, 60, 60) : RGB(80, 80, 80);
			COLORREF btnColor_Selected = RGB(63, 98, 120);
			HBRUSH hBGBrush = CreateSolidBrush(bgColor);
			HBRUSH hBrush = CreateSolidBrush(btnColor);

			if (lpDraw->CtlID == ID_BTN_PLAY)
			{
				if (CEngineEditor::GetInstance().isPlaying())
					hBrush = CreateSolidBrush(btnColor_Selected);
			}
			else if (lpDraw->CtlID == ID_BTN_PAUSE)
			{
				if (CEngineEditor::GetInstance().isPaused())
					hBrush = CreateSolidBrush(btnColor_Selected);
			}

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
			if (lpDraw->CtlID == ID_BTN_PLAY)
				text = CEngineEditor::GetInstance().isPlaying() ? L"■" : L"▶";
			else if (lpDraw->CtlID == ID_BTN_PAUSE)
				text = L"❚❚";
			else if (lpDraw->CtlID == ID_BTN_NEXTFRAME)
				text = L"▶❚";

			if (lpDraw->CtlID == ID_BTN_PLAY)
			{
				RECT textRect = rc;
				OffsetRect(&textRect, 0, 0);

				HFONT hFont = CEngineEditor::CreateDefaultFont(L"Segoe UI Variable", 12);

				HGDIOBJ oldFont = SelectObject(hdc, hFont);

				DrawText(hdc, text, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				SelectObject(hdc, oldFont);
				DeleteObject(hFont);
			}
			else if (lpDraw->CtlID == ID_BTN_PAUSE)
			{
				RECT textRect = rc;
				OffsetRect(&textRect, 0, -1);

				HFONT hFont = CEngineEditor::CreateDefaultFont(L"Segoe UI Variable", 20);

				HGDIOBJ oldFont = SelectObject(hdc, hFont);

				DrawText(hdc, text, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				SelectObject(hdc, oldFont);
				DeleteObject(hFont);
			}
			else if (lpDraw->CtlID == ID_BTN_NEXTFRAME)
			{
				RECT textRect = rc;
				OffsetRect(&textRect, 0, 0);

				HFONT hFont = CEngineEditor::CreateDefaultFont(L"Arial", 16);

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

	return TRUE;
}
