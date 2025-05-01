#include "CEngineEditor.h"
#include "CScreen.h"
#include "CInput.h"
#include "CDebug.h"
#include "CManagement.h"
#include <winuser.h>

CEngineEditor::CEngineEditor()
	: m_sEngineOptions({})
	, m_hInst(nullptr)
	, m_mWindowList({})
	, m_bPlaying(false)
	, m_bPaused(false)
	, m_bNextFrame(false)
	, m_pSelectedGameObject(nullptr)
{
	m_strName = L"Engine Editor";
}

CEngineEditor::~CEngineEditor()
{
	Release();
}

void CEngineEditor::Release()
{
	Safe_Release(m_pSelectedGameObject);

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
	scenewcex.hIconSm = NULL;
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
	gamewcex.hIconSm = NULL;
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
	hierachywcex.hIconSm = NULL;
	if (!RegisterClassExW(&hierachywcex))
		return 0;

	// Project Window
	WNDCLASSEXW projectwcex = {};
	projectwcex.lpszClassName = L"ProjectWindowClass";
	projectwcex.lpszMenuName = nullptr;
	projectwcex.cbSize = sizeof(WNDCLASSEX);
	projectwcex.lpfnWndProc = _wndPrc;
	projectwcex.cbClsExtra = 0;
	projectwcex.cbWndExtra = 0;
	projectwcex.hInstance = hInstance;
	projectwcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
	projectwcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	projectwcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	projectwcex.hIconSm = NULL;
	if (!RegisterClassExW(&projectwcex))
		return 0;

	// Inspector Window
	WNDCLASSEXW inspectorwcex = {};
	inspectorwcex.lpszClassName = L"InspectorWindowClass";
	inspectorwcex.lpszMenuName = nullptr;
	inspectorwcex.cbSize = sizeof(WNDCLASSEX);
	inspectorwcex.lpfnWndProc = _wndPrc;
	inspectorwcex.cbClsExtra = 0;
	inspectorwcex.cbWndExtra = 0;
	inspectorwcex.hInstance = hInstance;
	inspectorwcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
	inspectorwcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	inspectorwcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	inspectorwcex.hIconSm = NULL;
	if (!RegisterClassExW(&inspectorwcex))
		return 0;

	return 1;
}

LRESULT CEngineEditor::WndProcHandle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_MOUSEWHEEL:
	{
		CInput::GetInstance().OnMouseWheel(wParam);
	}
	break;

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

void CEngineEditor::Update()
{
	for (TRAVERSAL_ITER(m_mWindowList, it))
		(*it).second->Update();

	_bool isScene = ((GetForegroundWindow() == FindWindowHandle(L"Scene")) || 
		(GetForegroundWindow() == FindWindowHandle(L"Hierachy")));

	if (isScene)
	{
		if (CInput::GetInstance().GetKeyDown_Editor(KEY_DELETE))
		{
			if (m_pSelectedGameObject)
				CGameObject::Destroy(m_pSelectedGameObject);
		}
	}
}

void CEngineEditor::LateUpdate()
{
}

HWND CEngineEditor::FindWindowHandle(const wstring _window)
{
	auto it = m_mWHandleList.find(_window);

	if (it == m_mWHandleList.end())
		return nullptr;

	return it->second;
}

void CEngineEditor::RemoveBtnsAndRoundedCorners(HWND _hWnd)
{
	COLORREF titleColor = CEngineEditor::GetInstance().getOptions().s_baseColor.rColor();
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

void CEngineEditor::SelectGameObject(CGameObject* _gameObject)
{
	if (m_pSelectedGameObject)
		Safe_Release(m_pSelectedGameObject);

	m_pSelectedGameObject = nullptr;

	if (_gameObject)
	{
		m_pSelectedGameObject = _gameObject;
		_gameObject->AddRef();
	}

	getWindow<CInspectorWindow>()->ViewTargetInfor_GameObject(_gameObject);
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
