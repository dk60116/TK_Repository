#include "CScreen.h"
#include "CManagement.h"

CScreen::CScreen()
	: m_hInstance(nullptr)
	, m_hMainWnd(nullptr)
	, m_hSceneWnd(nullptr)
	, m_hGameWnd(nullptr)
	, m_v2GameResolution(vector2(960, 540))
	, m_bFullScreen(false)
	, m_iDPI(0)
{
}

CScreen::~CScreen()
{
}

void CScreen::Start_Window(HINSTANCE _hInst, int _cmdShow)
{
	DEVMODE devMode = {};
	devMode.dmSize = sizeof(DEVMODE);
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);

	int screenWidth = devMode.dmPelsWidth;
	int screenHeight = devMode.dmPelsHeight;

	m_hInstance = _hInst;

	RECT rc{ 0, 0, screenWidth, screenHeight };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	int offset = MAINTOPBARHEIGHT;

	m_hMainWnd = CreateWindowW(L"MaindowClass", L"Main",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		rc.left, rc.top + offset,
		rc.right - rc.left, rc.bottom - rc.top + offset,
		nullptr, nullptr, _hInst, nullptr);

	RECT windowRect;
	GetWindowRect(m_hMainWnd, &windowRect);

	int clientHeight = windowRect.bottom - windowRect.top;

	RECT grc;
	GetClientRect(m_hMainWnd, &grc);

	POINT pt = { 0 , 14 };
	ClientToScreen(m_hMainWnd, &pt);

	int titleBarHeight = GetSystemMetrics(SM_CYCAPTION);
	int frameHeight = GetSystemMetrics(SM_CYFRAME);
	int menuBarHeight = GetSystemMetrics(SM_CYMENU);

	int menuHeight = titleBarHeight + frameHeight + menuBarHeight;

	int width = grc.right / 2;
	int height = (clientHeight / 2) - (menuHeight * 2) - (15);

	RECT rcClient, rcWindow;
	GetClientRect(m_hMainWnd, &rcClient);
	GetWindowRect(m_hMainWnd, &rcWindow);

	int offsetX = (rcWindow.left - rcClient.left);
	int offsetY = (rcWindow.top - rcClient.top);

	m_hSceneWnd = CreateWindowW(L"SceneWindowClass", L"Scene",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		pt.x + offsetX,
		pt.y + offsetY,
		width,
		height,
		m_hMainWnd, nullptr, _hInst, nullptr);

	RemoveBtnsAndRoundedCorners(m_hSceneWnd);
	UpdateSceneResolution(width, height - 30);

	m_hGameWnd = CreateWindowW(L"GameWindowClass", L"Game",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		pt.x + offsetX, 
		pt.y + offsetY + height + offsetX,
		width,
		height,
		m_hMainWnd, nullptr, _hInst, nullptr);

	RemoveBtnsAndRoundedCorners(m_hGameWnd);
	UpdateGameResolution(width, height - 30);

	ShowWindow(m_hMainWnd, SW_SHOWMAXIMIZED);
	UpdateWindow(m_hMainWnd);
}

void CScreen::UpdateSceneResolution(const _int& _width, const _int& _height)
{
	m_v2SceneResolution = vector2Int(_width, _height);
}

void CScreen::UpdateGameResolution(const _int& _width, const _int& _height)
{
	m_v2GameResolution = vector2Int(_width, _height);
}

void CScreen::RemoveBtnsAndRoundedCorners(HWND _hWnd)
{
	LONG style = GetWindowLong(_hWnd, GWL_STYLE);

	style &= ~WS_MINIMIZEBOX;
	style &= ~WS_MAXIMIZEBOX;
	style &= ~WS_SYSMENU;

	SetWindowLong(_hWnd, GWL_STYLE, style);

	SetWindowPos(_hWnd, NULL, 0, 0, 0, 0,
		SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

	DWM_WINDOW_CORNER_PREFERENCE preference = DWMWCP_DONOTROUND;

	HRESULT hr = DwmSetWindowAttribute
	(
		_hWnd,
		DWMWA_WINDOW_CORNER_PREFERENCE, // 33
		&preference,
		sizeof(preference)
	);
}
