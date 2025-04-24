#include "CEngineEditor.h"
#include "CScreen.h"
#include "CManagement.h"
#include <winuser.h>

CEngineEditor::CEngineEditor()
	: m_hInst(nullptr)
	, m_bPlaying(false)
	, m_bPaused(false)
	, m_bNextFrame(false)
	, m_hTop_Scene(nullptr)
	, m_hTop_Game(nullptr)
{
}

CEngineEditor::~CEngineEditor()
{
	Release();
}

void CEngineEditor::Init_Main(HINSTANCE _hInst, HWND _mainWnd)
{
	m_hInst = _hInst;

	HFONT hFont14 = CreateDefaultFont(L"Arial", 18);
	HFONT hFont16 = CreateDefaultFont(L"Arial", 16, TRUE);

	//SendMessageW(m_hBtnPlay, WM_SETFONT, (WPARAM)hFont14, TRUE);
	//SendMessageW(m_hBtnPause, WM_SETFONT, (WPARAM)hFont16, TRUE);
	//SendMessageW(m_hBtnNextFrame, WM_SETFONT, (WPARAM)hFont14, TRUE);
	//
	//EnableWindow(m_hBtnPlay, TRUE);
	//EnableWindow(m_hBtnPause, m_bPlaying);
	//EnableWindow(m_hBtnNextFrame, m_bPaused);
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

void CEngineEditor::Release()
{
	for (TRAVERSAL_ITER(m_mWindowList, it))
	{
		(*it).second->Destroy();
		Safe_Release((*it).second);
	}

	m_mWindowList.clear();
}

ATOM CEngineEditor::MyRegisterClass(HINSTANCE hInstance, WNDPROC _wndPrc)
{
	// Main Window
	WNDCLASSEXW basewcex = {};
	basewcex.lpszClassName = L"MaindowClass";
	basewcex.lpszMenuName = MAKEINTRESOURCE(IDC_CLIENT);
	basewcex.cbSize = sizeof(WNDCLASSEX);
	basewcex.style = CS_HREDRAW | CS_VREDRAW;
	basewcex.lpfnWndProc = _wndPrc;
	basewcex.cbClsExtra = 0;
	basewcex.cbWndExtra = 0;
	basewcex.hInstance = hInstance;
	basewcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
	basewcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	basewcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	basewcex.hIconSm = LoadIcon(basewcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	if (!RegisterClassExW(&basewcex))
		return 0;

	// Scene Window
	WNDCLASSEXW scenewcex = {};
	scenewcex.lpszClassName = L"SceneWindowClass";
	scenewcex.lpszMenuName = NULL;
	scenewcex.cbSize = sizeof(WNDCLASSEX);
	scenewcex.style = CS_HREDRAW | CS_VREDRAW;
	scenewcex.lpfnWndProc = _wndPrc;
	scenewcex.cbClsExtra = 0;
	scenewcex.cbWndExtra = 0;
	scenewcex.hInstance = hInstance;
	scenewcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
	scenewcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	scenewcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	scenewcex.hIconSm = LoadIcon(basewcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	if (!RegisterClassExW(&scenewcex))
		return 0;

	// Game Window
	WNDCLASSEXW gamewcex = {};
	gamewcex.lpszClassName = L"GameWindowClass";
	gamewcex.lpszMenuName = nullptr;
	gamewcex.cbSize = sizeof(WNDCLASSEX);
	gamewcex.style = CS_HREDRAW | CS_VREDRAW;
	gamewcex.lpfnWndProc = _wndPrc;
	gamewcex.cbClsExtra = 0;
	gamewcex.cbWndExtra = 0;
	gamewcex.hInstance = hInstance;
	gamewcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
	gamewcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	gamewcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	gamewcex.hIconSm = LoadIcon(basewcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	if (!RegisterClassExW(&gamewcex))
		return 0;

	// Hierachy Window
	WNDCLASSEXW hierachywcex = {};
	hierachywcex.lpszClassName = L"HierachyWindowClass";
	hierachywcex.lpszMenuName = nullptr;
	hierachywcex.cbSize = sizeof(WNDCLASSEX);
	hierachywcex.lpfnWndProc = _wndPrc;
	hierachywcex.cbClsExtra = 0;
	hierachywcex.cbWndExtra = 0;
	hierachywcex.hInstance = hInstance;
	hierachywcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
	hierachywcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	hierachywcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	hierachywcex.hIconSm = LoadIcon(basewcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	if (!RegisterClassExW(&hierachywcex))
		return 0;

	return 1;
}

LRESULT CEngineEditor::WndProcHandle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
	{
		RECT rcClient;
		GetClientRect(hWnd, &rcClient);
		_int width = rcClient.right - rcClient.left;
		_int height = rcClient.bottom - rcClient.top;

		for (TRAVERSAL_ITER(m_mWindowList, it))
			(*it).second->UpdateResolution(hWnd, vector2Int(width, height));
	}
	break;
	}

	for (TRAVERSAL_ITER(m_mWindowList, it))
		(*it).second->WndProcHandle(hWnd, message, wParam, lParam);

	return TRUE;
}

void CEngineEditor::UpdateSceneResolution(const vector2Int _resolution)
{
	MoveWindow
	(
		m_hTop_Scene,
		0, 0,
		_resolution.x, CHILDTOPBARHEIGHT,
		true
	);
}

CEditorWindow* CEngineEditor::getWindow(wstring _name)
{
	auto it = m_mWindowList.find(_name);

	if (it != m_mWindowList.end())
		return it->second;

	return nullptr;
}

HFONT CEngineEditor::CreateDefaultFont(LPCWSTR _font, const _int _size, const _bool _bold)
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
