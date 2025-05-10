#include "CEngineEditor.h"
#include "Screen.h"
#include "CInput.h"
#include "CDebug.h"
#include "Management.h"
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

	RegisterBaseColorPanelClass();

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
	COLORREF titleColor = CEngineEditor::GetInstance().getOptions().baseColor.rColor();
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

void CEngineEditor::BeautifyEdit(HWND _hEdit)
{
	/* 1) 기본 테마·테두리 끄기 */
	SetWindowTheme(_hEdit, L"", L"");

	LONG style = GetWindowLong(_hEdit, GWL_STYLE);
	style &= ~(WS_BORDER);                   // ← 빠졌던 기본 테두리 제거
	SetWindowLong(_hEdit, GWL_STYLE, style);

	LONG_PTR ex = GetWindowLongPtr(_hEdit, GWL_EXSTYLE);
	ex &= ~(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
	SetWindowLongPtr(_hEdit, GWL_EXSTYLE, ex);

	SetWindowPos(_hEdit, nullptr, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
		SWP_FRAMECHANGED | SWP_NOACTIVATE);

	/* 2) 둥근 Region 지정 */
	RECT rc; GetClientRect(_hEdit, &rc);
	constexpr int R = 8;                       // 반경을 조금 키워 눈에 띄게
	HRGN rgn = CreateRoundRectRgn(0, 0,
		rc.right + 1, rc.bottom + 1, R, R);
	SetWindowRgn(_hEdit, rgn, TRUE);

	/* 3) 서브클래싱 – NC 페인트 차단 */
	SetWindowSubclass(_hEdit, EditSubclassProc, 0, 0);
}

LRESULT CEngineEditor::EditSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR)
{
	switch (msg)
	{
	case WM_NCPAINT:         // ⬅ 비‑클라이언트(테두리) 칠하기 차단
		return 0;            //   → 사각형 테두리가 더 이상 그려지지 않음

	case WM_SIZE:            // 크기 변화 시 Region 재설정
	{
		RECT rc; GetClientRect(hWnd, &rc);
		constexpr int R = 8;
		HRGN r = CreateRoundRectRgn(0, 0, rc.right + 1, rc.bottom + 1, R, R);
		SetWindowRgn(hWnd, r, TRUE);
		break;
	}

	case WM_ERASEBKGND:      // 다크 배경 직접 칠하기
	{
		HDC hdc = (HDC)wParam;
		RECT rc; GetClientRect(hWnd, &rc);
		FillRect(hdc, &rc, GetStockBrush(DKGRAY_BRUSH));
		return 1;
	}

	case WM_NCDESTROY:
		RemoveWindowSubclass(hWnd, EditSubclassProc, 0);
		break;
	}
	return DefSubclassProc(hWnd, msg, wParam, lParam);
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

void CEngineEditor::RegisterBaseColorPanelClass()
{
	WNDCLASS wc{};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = [](HWND h, UINT m, WPARAM w, LPARAM l)->LRESULT
		{
			switch (m)
			{
			case WM_NCCREATE:
				SetWindowLongPtr(h, GWLP_USERDATA,
					(LONG_PTR)CreateSolidBrush(CEngineEditor::GetInstance().getOptions().baseColor.rColor()));
				return TRUE;

			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC dc = BeginPaint(h, &ps);

				HBRUSH bg = (HBRUSH)GetWindowLongPtr(h, GWLP_USERDATA);
				HBRUSH old = (HBRUSH)SelectObject(dc, bg);

				//RECT rc; GetClientRect(h, &rc);
				//const int rad = 6;              
				//RoundRect(dc, rc.left, rc.top, rc.right, rc.bottom, rad, rad);

				SelectObject(dc, old);
				EndPaint(h, &ps);
				return 0;
			}
			case WM_NCDESTROY:
				DeleteObject((HBRUSH)GetWindowLongPtr(h, GWLP_USERDATA));
				break;
			}
			return DefWindowProc(h, m, w, l);
		};

	wc.hInstance = GetModuleHandle(nullptr);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = L"BASECOLORCLASS";
	RegisterClass(&wc);
}
