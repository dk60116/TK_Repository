#include "CEngineEditor.h"
#include "CScreen.h"

CEngineEditor::CEngineEditor()
	: m_hInst(nullptr)
	, m_hMainWnd(nullptr)
	, m_bPaused(false)
	, m_hTopBar(nullptr)
	, m_hBtnPause(nullptr)
	, m_hBtnStop(nullptr)
	, m_hBtnNextFrame(nullptr)
{
}

CEngineEditor::~CEngineEditor()
{
}

void CEngineEditor::Init(HINSTANCE _hInst, HWND _mainWnd)
{
	m_hMainWnd = _mainWnd;
	m_hInst = _hInst;
	m_bPaused = false;

	int screenX = CScreen::GetInstance().getResolution().x;
	int screenXCenter = screenX / 2;

	// 상단바 영역
	m_hTopBar = CreateWindowW(L"STATIC", nullptr,
		WS_VISIBLE | WS_CHILD,
		0, 0, screenX, 30,
		m_hMainWnd, nullptr, m_hInst, nullptr);

	// 일시정지/재생 버튼
	m_hBtnPause = CreateWindowW(L"BUTTON", L"II", WS_VISIBLE | WS_CHILD,
		screenXCenter - 40 - 20, 1, 40, 28,
		m_hMainWnd, (HMENU)ID_BTN_PAUSE, m_hInst, nullptr);

	// 종료 버튼
	m_hBtnStop = CreateWindowW(L"BUTTON", L"■", WS_VISIBLE | WS_CHILD,
		screenXCenter - 20, 1, 40, 28,
		m_hMainWnd, (HMENU)ID_BTN_STOP, m_hInst, nullptr);

	// 다음 프레임 버튼
	m_hBtnNextFrame = CreateWindowW(L"BUTTON", L"▶I", WS_VISIBLE | WS_CHILD,
		screenXCenter + 20, 1, 40, 28,
		m_hMainWnd, (HMENU)ID_BTN_NEXTFRAME, m_hInst, nullptr);

	EnableWindow(m_hBtnNextFrame, m_bPaused);
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

void CEngineEditor::UpdateResolution()
{
	int screenX = CScreen::GetInstance().getResolution().x;
	int screenXCenter = screenX / 2;

	MoveWindow(m_hTopBar,
		0, 0,
		CScreen::GetInstance().getResolution().x, 30,
		true);

	MoveWindow(m_hBtnPause,
		screenXCenter - 40 - 20, 1,
		40, 28,
		true);

	MoveWindow(m_hBtnStop,
		screenXCenter - 20, 1,
		40, 28,
		true);

	MoveWindow(m_hBtnNextFrame,
		screenXCenter + 20, 1,
		40, 28,
		true);
}
