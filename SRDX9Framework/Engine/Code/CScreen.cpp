#include "CScreen.h"
#include "CManagement.h"
#include "CHierachyWindow.h"

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

	_int sceneWidth = _int((clientWidth * 0.45f) - clientPoint.x * 2);
	_int sceneHeight = _int((clientHeight / 2) - menuHeight - 1) + 2;

	HWND sceneWnd = CreateWindowW(L"SceneWindowClass", L"𐄹 Scene",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		clientPoint.x,
		clientPoint.y,
		sceneWidth,
		sceneHeight,
		mainWnd, nullptr, _hInst, nullptr);

	m_mWHandleList.insert({ L"Scene", sceneWnd });

	RECT sceneRect = {};
	GetClientRect(sceneWnd, &sceneRect);

	RemoveBtnsAndRoundedCorners(sceneWnd);
	UpdateSceneResolution(sceneWidth, sceneHeight);

	POINT scenePT = { 0, sceneRect.bottom };
	ClientToScreen(sceneWnd, &scenePT);

	_int sceneBottomY = scenePT.y;

	_int gameWidth = sceneWidth;
	_int gameHeight = rcClient.bottom - scenePT.y + captionHeight;

	HWND gameWnd = CreateWindowW(L"GameWindowClass", L"🎮 Game",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		clientPoint.x,
		sceneBottomY,
		gameWidth,
		gameHeight,
		mainWnd, nullptr, _hInst, nullptr);

	m_mWHandleList.insert({ L"Game", gameWnd });

	RECT gameRect = {};

	GetClientRect(gameWnd, &gameRect);

	POINT gamePT = { 0, gameRect.bottom };
	ClientToScreen(gameWnd, &gamePT);

	RemoveBtnsAndRoundedCorners(gameWnd);
	UpdateGameResolution(gameWidth, gameHeight);

	_int hierachyWidth = _int((clientWidth * 0.2f) - clientPoint.x * 2);
	_int hierachyHeight = _int(gamePT.y - clientPoint.y - offsetY * 2);

	HWND hierachyWnd = CreateWindowW(L"HierachyWindowClass", L"☰ Hierachy",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		sceneRect.right + offsetX + 2,
		clientPoint.y,
		hierachyWidth,
		hierachyHeight,
		mainWnd, nullptr, _hInst, nullptr);

	m_mWHandleList.insert({ L"Hierachy", hierachyWnd });
	
	CHierachyWindow::GetInstance().Init(hierachyWnd, vector2Int(hierachyWidth, hierachyHeight));

	RemoveBtnsAndRoundedCorners(hierachyWnd);
}

HWND CScreen::getWindowHandle(wstring _window)
{
	auto it = m_mWHandleList.find(_window);

	if (it != m_mWHandleList.end())
		return it->second;
	
	return nullptr;
}

void CScreen::UpdateSceneResolution(const _int _width, const _int _height)
{
	m_v2SceneResolution = vector2Int(_width, _height - CHILDTOPBARHEIGHT);
}

void CScreen::UpdateGameResolution(const _int _width, const _int _height)
{
	m_v2GameResolution = vector2Int(_width, _height - CHILDTOPBARHEIGHT);
}

void CScreen::RemoveBtnsAndRoundedCorners(HWND _hWnd)
{
	//BOOL value = TRUE;
	//DwmSetWindowAttribute(_hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));

	COLORREF titleColor = RGB(56, 56, 56);
	DwmSetWindowAttribute(_hWnd, DWMWA_CAPTION_COLOR, &titleColor, sizeof(titleColor));

	LONG style = GetWindowLong(_hWnd, GWL_STYLE);

	style &= ~WS_MINIMIZEBOX;
	style &= ~WS_MAXIMIZEBOX;
	style &= ~WS_SYSMENU;

	SetWindowLong(_hWnd, GWL_STYLE, style);

	SetWindowPos(_hWnd, NULL, 0, 0, 0, 0,
		SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

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
