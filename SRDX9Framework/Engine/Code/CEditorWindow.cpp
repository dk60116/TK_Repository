#include "CEditorWindow.h"
#include "CEngineEditor.h"

CEditorWindow::CEditorWindow()
	: m_hWnd(nullptr)
	, m_hTopBar(nullptr)
	, m_iTopBarHeight(CHILDTOPBARHEIGHT)
	, m_v2Resolution(vector2Int::zero())
	, m_sClinentRect({})
	, m_sWindowRect({})
	, m_bHaveWindowBar(false)
{
}

CEditorWindow::~CEditorWindow()
{
}

HRESULT CEditorWindow::Init(HWND _hWnd, const vector2Int _size)
{
	m_hWnd = _hWnd;
	m_v2Resolution = _size;

	if (!m_bHaveWindowBar)
		CEngineEditor::GetInstance().RemoveBtnsAndRoundedCorners(_hWnd);

	GetClientRect(_hWnd, &m_sClinentRect);
	GetWindowRect(_hWnd, &m_sWindowRect);

	// 상단바 영역
	m_hTopBar = CreateWindowW(L"STATIC", L"TopBar",
		WS_VISIBLE | WS_CHILD,
		0, 0, _size.x, m_iTopBarHeight,
		_hWnd, nullptr, CEngineEditor::GetInstance().getHInstance(), nullptr);

	if (!m_hTopBar)
		return E_FAIL;

	return S_OK;
}

void CEditorWindow::UpdateResolution(HWND _target, const vector2Int _resolution)
{
	m_v2Resolution = _resolution - vector2Int(0, (int)m_iTopBarHeight);
	
	MoveWindow(m_hTopBar, 0, 0, _resolution.x, m_iTopBarHeight, TRUE);
}

void CEditorWindow::Destroy()
{
	Release();
}

LRESULT CEditorWindow::WndProcHandle(HWND hWnd, UINT _message, WPARAM wParam, LPARAM lParam)
{
	switch (_message)
	{
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		HWND hStatic = (HWND)lParam;

		if (hStatic == m_hTopBar)
		{
			SetBkMode(hdcStatic, TRANSPARENT);
			SetBkColor(hdcStatic, RGB(0, 0, 0));
			static HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
			return (INT_PTR)hBlackBrush;
		}
	}
	break;
	}

	return TRUE;
}
