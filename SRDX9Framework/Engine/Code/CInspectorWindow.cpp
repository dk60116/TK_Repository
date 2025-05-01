#include "CInspectorWindow.h"

CInspectorWindow::CInspectorWindow()
	: m_sOptinos({})
	, m_vContentsWindows({})
	, m_pViewGameObject(nullptr)
	, m_iTotalHeight(0)
	, m_iScrollPos(0)
{
	m_strName = L"Editor:Inspector Window";
}

CInspectorWindow::~CInspectorWindow()
{
	Destroy();
}

HRESULT CInspectorWindow::Init(HWND _hWnd, vector2Int _size, _bool _isBase)
{
	__super::Init(_hWnd, _size, _isBase);

	return S_OK;
}

void CInspectorWindow::Render()
{
}

void CInspectorWindow::Update()
{
}

void CInspectorWindow::UpdateResolution(HWND _target, vector2Int _resolution)
{
}

LRESULT CInspectorWindow::WndProcHandle(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	switch (_message)
	{
	case WM_SIZE:
		UpdateScrollInfo();
		break;

	case WM_MOUSEWHEEL:
	{
		short z = GET_WHEEL_DELTA_WPARAM(_wParam);   // ±120, ±240 …
		int steps = z / WHEEL_DELTA;                 // ±1, ±2 …
		for (int n = 0; n < std::abs(steps); ++n)
			SendMessage(_hWnd, WM_VSCROLL,
				MAKEWPARAM(steps > 0 ? SB_LINEUP : SB_LINEDOWN, 0), 0);
		return 0;
	}
		break;

	case WM_VSCROLL:
	{
		if (GetForegroundWindow() == m_hWnd)
		{
			SCROLLINFO si{ sizeof(si), SIF_ALL };
			GetScrollInfo(m_hWnd, SB_VERT, &si);
			int oldPos = si.nPos;

			switch (LOWORD(_wParam))
			{
			case SB_LINEUP:      si.nPos -= 40;            break;   // 한 줄(픽셀)
			case SB_LINEDOWN:    si.nPos += 40;            break;
			case SB_PAGEUP:      si.nPos -= si.nPage;      break;
			case SB_PAGEDOWN:    si.nPos += si.nPage;      break;
			case SB_THUMBTRACK:  si.nPos = si.nTrackPos;  break;
			default: return 0;
			}

			si.fMask = SIF_POS;
			SetScrollInfo(m_hWnd, SB_VERT, &si, TRUE);
			GetScrollInfo(m_hWnd, SB_VERT, &si);
			m_iScrollPos = si.nPos;

			int dy = oldPos - si.nPos;                     // (+) 위로 스크롤
			ScrollWindowEx
			(m_hWnd, 0, dy,
				nullptr, nullptr,
				nullptr, nullptr,
				SW_SCROLLCHILDREN | SW_INVALIDATE | SW_ERASE
			);
		}
		return 0;
	}
	}
	
	return __super::WndProcHandle(_hWnd, _message, _wParam, _lParam);
}

void CInspectorWindow::Destroy()
{
	__super::Destroy();

	if (m_pViewGameObject)
		Safe_Release(m_pViewGameObject);
}

void CInspectorWindow::ClearComponents()
{
	for (TRAVERSAL_ITER(m_vContentsWindows, it))
	{
		if (IsWindow(*it))
			DestroyWindow(*it);
	}

	m_vContentsWindows.clear();

	m_iTotalHeight = 0;
	m_iScrollPos = 0;
}

void CInspectorWindow::ViewTargetInfor_GameObject(CGameObject* _target)
{
	ClearComponents();

	UpdateScrollInfo();              

	const _int clientW = m_sClinentRect.right;      

	SCROLLINFO si{ sizeof(si), SIF_RANGE | SIF_PAGE };
	GetScrollInfo(m_hWnd, SB_VERT, &si);
	const _bool hasScroll = (si.nMax - si.nMin + 1) > si.nPage;

	const _int margin = 4;                
	const _int sbWidth = hasScroll ? GetSystemMetrics(SM_CXVSCROLL) : 0;
	const _int itemW = clientW - sbWidth - margin;     

	int i = 0;

	HWND top = CreateWindowEx(
		0, L"STATIC", nullptr,
		WS_CHILD | WS_VISIBLE,
		margin / 2, m_sOptinos.contstsBarHeight * i,
		itemW, m_sOptinos.topHeight,
		m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr);

	m_vContentsWindows.push_back(top);
	m_iTotalHeight += m_sOptinos.topHeight;

	for (CComponent* c : _target->getComponentList())
	{
		HWND h = CreateWindowEx
		(
			0, L"STATIC", nullptr,
			WS_CHILD | WS_VISIBLE,
			margin / 2, 
			(m_sOptinos.contstsBarHeight * i) + m_sOptinos.topHeight,
			itemW, m_sOptinos.contstsBarHeight,
			m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr
		);

		m_vContentsWindows.push_back(h);
		m_iTotalHeight += m_sOptinos.contstsBarHeight;
		++i;
	}

	UpdateScrollInfo();            
}

void CInspectorWindow::UpdateScrollInfo()
{
	RECT rc;  GetClientRect(m_hWnd, &rc);

	const int viewH = rc.bottom;           
	const int nRange = max(m_iTotalHeight, 1);
	const int maxPos = max(m_iTotalHeight - viewH, 0);

	SCROLLINFO si{ sizeof(si) };
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = 0;
	si.nMax = nRange - 1;
	si.nPage = viewH;
	si.nPos = clamp(m_iScrollPos, 0, maxPos);
	SetScrollInfo(m_hWnd, SB_VERT, &si, TRUE);
}
