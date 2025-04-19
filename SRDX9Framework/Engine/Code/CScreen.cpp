#include "CScreen.h"

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

void CScreen::Start_Window(LPWCH _winClass, HINSTANCE _hInst, int _cmdShow)
{
	DEVMODE devMode = {};
	devMode.dmSize = sizeof(DEVMODE);
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);

	int screenWidth = devMode.dmPelsWidth;
	int screenHeight = devMode.dmPelsHeight;

	m_hInstance = _hInst;

#ifdef _DEBUG
	RECT rc{ 0, 0, screenWidth, screenHeight };
#else
	RECT rc{ 0, 0, CScreen::GetInstance().getResolution().x, CScreen::GetInstance().getResolution().y };
#endif

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	int offset = 0;

#ifdef _DEBUG
	offset = 30;

	m_hMainWnd = CreateWindowW(_winClass, L"Main",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		rc.left, rc.top + offset + 10,
		rc.right - rc.left, rc.bottom - rc.top + offset - 10,
		nullptr, nullptr, _hInst, nullptr);
#endif

	RECT windowRect, clientRect;
	GetWindowRect(m_hMainWnd, &windowRect);
	GetClientRect(m_hMainWnd, &clientRect);

	int clientHeight = clientRect.bottom - clientRect.top;

	int windowHeight = windowRect.bottom - windowRect.top;

	RECT grc;

	GetClientRect(m_hMainWnd, &grc);

	POINT pt = { -10 , 12 };
	ClientToScreen(m_hMainWnd, &pt);

	int width = grc.right / 2;
	int height = grc.bottom;

	int titleBarHeight = GetSystemMetrics(SM_CYCAPTION);
	int frameHeight = GetSystemMetrics(SM_CYFRAME);
	int menuBarHeight = GetSystemMetrics(SM_CYMENU);

	int totalHeight = titleBarHeight + frameHeight + menuBarHeight;

	m_hSceneWnd = CreateWindowW(_winClass, L"Scene",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		pt.x, pt.y,
		width,
		(clientHeight / 2) - totalHeight - 5,
		m_hMainWnd, nullptr, _hInst, nullptr);

	UpdateSceneResolution(width, (clientHeight / 2) - totalHeight - 5);

	m_hGameWnd = CreateWindowW(_winClass, L"Game",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		pt.x, offset + clientHeight / 2 - totalHeight + offset,
		width,
		(clientHeight / 2) - totalHeight - 5,
		m_hMainWnd, nullptr, _hInst, nullptr);

	UpdateSceneResolution(width, (clientHeight / 2) - totalHeight - 5);

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
