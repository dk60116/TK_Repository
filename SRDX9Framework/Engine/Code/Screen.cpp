#include "Screen.h"
#include "Management.h"
#include "EngineEditor.h"

CScreen::CScreen()
	: m_hInstance(nullptr)
	, m_v2GameResolution(vector2(0, 0))
	, m_bFullScreen(false)
{
}

CScreen::~CScreen()
{
}

HRESULT CScreen::Start_Window(HINSTANCE _hInst, int _cmdShow)
{
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

	HWND mainWnd = CreateWindowW
	(
		L"MaindowClass", L"Main",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		winRect.left,
		winRect.top,
		winWidth,
		winHeight,
		nullptr, nullptr, _hInst, nullptr
	);

	if (FAILED(CEngineEditor::GetInstance().CreateCustomWindow<CBaseWindow>(mainWnd, L"Base", vector2Int(winWidth, winHeight))))
		return E_FAIL;

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

	if (FAILED(CEngineEditor::GetInstance().CreateCustomWindow<CSceneWindow>(sceneWnd, L"Scene", vector2Int(sceneWidth, sceneHeight))))
		return E_FAIL;

	RECT sceneRect = {};
	GetClientRect(sceneWnd, &sceneRect);

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

	if (FAILED(CEngineEditor::GetInstance().CreateCustomWindow<CGameWindow>(gameWnd, L"Game", vector2Int(gameWidth, gameHeight))))
		return E_FAIL;

	RECT gameRect = {};
	GetClientRect(gameWnd, &gameRect);

	POINT gamePT = { 0, gameRect.bottom };
	ClientToScreen(gameWnd, &gamePT);

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

	if (FAILED(CEngineEditor::GetInstance().CreateCustomWindow<CHierachyWindow>(hierachyWnd, L"Hierachy", vector2Int(hierachyWidth, hierachyHeight))))
		return E_FAIL;

	RECT hierachyRect = {};
	GetClientRect(hierachyWnd, &hierachyRect);

	POINT hierachyPT = { hierachyRect.right, hierachyRect.bottom };
	ClientToScreen(hierachyWnd, &hierachyPT);

	_int projectWidth = _int(hierachyWidth * 0.75f);
	_int projectHeight = _int(gamePT.y - clientPoint.y - offsetY * 2);

	HWND projectWnd = CreateWindowW(L"ProjectWindowClass", L"🗀 Project",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		hierachyPT.x + offsetX + 2,
		clientPoint.y,
		projectWidth,
		projectHeight,
		mainWnd, nullptr, _hInst, nullptr);

	CEngineEditor::RemoveBtnsAndRoundedCorners(projectWnd);

	RECT projectRect = {};
	GetClientRect(projectWnd, &projectRect);

	POINT projectPT = { projectRect.right, projectRect.bottom };
	ClientToScreen(projectWnd, &projectPT);

	_int inspectorWidth = _int(winWidth - (sceneWidth + hierachyWidth + projectWidth) - offsetX * 5 + 2);
	_int inspectorHeight = _int(projectPT.y - clientPoint.y - offsetY * 2);

	HWND inspectorWnd = CreateWindowW(L"InspectorWindowClass", L"ⓘ Inspector",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VSCROLL,
		projectPT.x + offsetX + 2,
		clientPoint.y,
		inspectorWidth,
		inspectorHeight,
		mainWnd, nullptr, _hInst, nullptr);

	if (FAILED(CEngineEditor::GetInstance().CreateCustomWindow<CInspectorWindow>(inspectorWnd, L"Inspector", vector2Int(inspectorWidth, inspectorHeight))))
		return E_FAIL;

	return S_OK;
}

void CScreen::UpdateSceneResolution(const UINT _width, const UINT _height)
{
	m_v2SceneResolution = vector2Int(_width, _height - CHILDTOPBARHEIGHT);
}

void CScreen::UpdateGameResolution(const UINT _width, const UINT _height)
{
	m_v2GameResolution = vector2Int(_width, _height - CHILDTOPBARHEIGHT);
}
