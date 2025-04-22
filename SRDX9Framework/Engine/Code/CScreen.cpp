#include "CScreen.h"
#include "CManagement.h"

CScreen::CScreen()
	: m_hInstance(nullptr)
	, m_mWHandleList({})
	, m_v2GameResolution(vector2(0, 0))
	, m_bFullScreen(false)
	, m_iDPI(0)
{
}

CScreen::~CScreen()
{
}

void CScreen::Start_Window(HINSTANCE _hInst, int _cmdShow)
{
	m_mWHandleList.clear();

	m_hInstance = _hInst;

	_int mainOffset = MAINTOPBARHEIGHT;
	_int childOffset = CHILDTOPBARHEIGHT;

	_int captionHeight = GetSystemMetrics(SM_CYCAPTION);
	_int frameHeight = GetSystemMetrics(SM_CYFRAME);
	_int menuBarHeight = GetSystemMetrics(SM_CYMENU);

	_int menuHeight = captionHeight + frameHeight;

	DEVMODE devMode = {};
	devMode.dmSize = sizeof(DEVMODE);
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);

	_int screenWidth = devMode.dmPelsWidth;
	_int screenHeight = devMode.dmPelsHeight;

	RECT winRect = { 0, menuHeight, screenWidth, screenHeight };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);

	_int winWidth = winRect.right - winRect.left;
	_int winHeight = winRect.bottom - winRect.top - mainOffset;

	HWND mainWnd = CreateWindowW(L"MaindowClass", L"Main",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		winRect.left,
		winRect.top,
		winWidth,
		winHeight,
		nullptr, nullptr, _hInst, nullptr);

	m_mWHandleList.insert({ L"Base", mainWnd });

	ShowWindow(mainWnd, SW_SHOW);
	UpdateWindow(mainWnd);

	RECT rcClient, rcWindow;
	GetClientRect(mainWnd, &rcClient);
	GetWindowRect(mainWnd, &rcWindow);

	int offsetX = (rcWindow.left - rcClient.left);
	int offsetY = (rcWindow.top - rcClient.top);

	POINT clientPoint = { offsetX, mainOffset };
	ClientToScreen(mainWnd, &clientPoint);

	_int clientWidth = rcClient.right - rcClient.left;
	_int clientHeight = rcClient.bottom - rcClient.top;

	_int width = (clientWidth / 2) - clientPoint.x * 2;
	_int height = (clientHeight / 2) - menuHeight;

	HWND sceneWnd = CreateWindowW(L"SceneWindowClass", L"Scene",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		clientPoint.x,
		clientPoint.y,
		width,
		height,
		mainWnd, nullptr, _hInst, nullptr);

	m_mWHandleList.insert({ L"Scene", sceneWnd });

	RECT sceneRect = {};
	GetClientRect(sceneWnd, &sceneRect);

	_int childMenuHeight = captionHeight + frameHeight;

	_int sceneWidth = sceneRect.right - sceneRect.left;
	_int sceneHeight = sceneRect.bottom - sceneRect.top;

	RemoveBtnsAndRoundedCorners(sceneWnd);
	UpdateSceneResolution(sceneWidth, sceneHeight - CHILDTOPBARHEIGHT);

	POINT scenePT = { 0, sceneRect.bottom };

	ClientToScreen(sceneWnd, &scenePT);

	_int sceneBottomY = scenePT.y + 1;

	HWND gameWnd = CreateWindowW(L"GameWindowClass", L"Game",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		clientPoint.x,
		sceneBottomY,
		width,
		height,
		mainWnd, nullptr, _hInst, nullptr);

	m_mWHandleList.insert({ L"Game", gameWnd });

	RECT gameRect = {};

	GetClientRect(gameWnd, &gameRect);

	_int gameWidth = gameRect.right - gameRect.left;
	_int gameHeight = gameRect.bottom - gameRect.top;

	RemoveBtnsAndRoundedCorners(gameWnd);
	UpdateGameResolution(gameWidth, gameHeight - CHILDTOPBARHEIGHT);
}

HWND CScreen::getWindowHandle(wstring _window)
{
	auto it = m_mWHandleList.find(_window);

	if (it != m_mWHandleList.end())
		return it->second;
	
	return nullptr;
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
