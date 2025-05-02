#include "CInspectorWindow.h"
#include "CEngineEditor.h"

CInspectorWindow::CInspectorWindow()
	: m_sOptinos({})
	, m_hEditName(nullptr)
	, m_vContentsWindows({})
	, m_vChildWindows({})
	, m_pViewGameObject(nullptr)
	, m_iTotalHeight(0)
	, m_iScrollPos(0)
	, m_hDarkBrush(CreateSolidBrush(RGB(55,55, 58)))
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
		if (!m_vContentsWindows.empty())
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
			case SB_LINEUP: si.nPos -= 40;            
				break;   // 한 줄(픽셀)
			case SB_LINEDOWN: si.nPos += 40;            
				break;
			case SB_PAGEUP: si.nPos -= si.nPage;      
				break;
			case SB_PAGEDOWN: si.nPos += si.nPage;      
				break;
			case SB_THUMBTRACK: si.nPos = si.nTrackPos;  
				break;
			default: 
				return 0;
			}

			si.fMask = SIF_POS;
			SetScrollInfo(m_hWnd, SB_VERT, &si, TRUE);
			GetScrollInfo(m_hWnd, SB_VERT, &si);
			m_iScrollPos = si.nPos;

			int dy = oldPos - si.nPos;                     // (+) 위로 스크롤
			ScrollWindowEx
			(
				m_hWnd, 0, dy,
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
	if (IsWindow(m_hEditName))
		DestroyWindow(m_hEditName);

	for (TRAVERSAL_ITER(m_vContentsWindows, it))
	{
		if (IsWindow(*it))
			DestroyWindow(*it);
	}

	for (TRAVERSAL_ITER(m_vChildWindows, it))
	{
		if (IsWindow(*it))
			DestroyWindow(*it);
	}

	m_vContentsWindows.clear();
	m_vChildWindows.clear();

	m_iTotalHeight = 0;
	m_iScrollPos = 0;

	UpdateScrollInfo();
}

void CInspectorWindow::ViewTargetInfor_GameObject(CGameObject* _target)
{
	ClearComponents();

	if (!_target)
		return;

	const _int margin = 4;
	const _int fieldH = 25;

	RECT rc; GetClientRect(m_hWnd, &rc);
	const _int clientW = rc.right;
	_int itemW = clientW - margin;
	_int x = margin / 2;
	_int y = 0;

	HWND top = CreateWindowEx(0, L"STATIC", nullptr,
		WS_CHILD | WS_VISIBLE,
		x, y, 
		itemW, m_sOptinos.topHeight,
		m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr);

	SetWindowLongPtr(top, GWLP_USERDATA, static_cast<LONG_PTR>(HWND_INPUTBOX));

	m_vContentsWindows.push_back(top);
	m_iTotalHeight += m_sOptinos.topHeight;
	y += m_sOptinos.topHeight;

	const _int lblW = 50;
	const _int edtH = 22;
	const _int edtW = itemW - lblW - 80;

	m_hEditName = CreateWindowEx
	(
		0, L"EDIT",
		_target->getName().c_str(),        
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_BORDER,
		4 + lblW, 8,
		edtW, edtH,
		m_hWnd, reinterpret_cast<HMENU>(1001),
		GetModuleHandle(nullptr), nullptr
	);

	LOGFONT lf{};
	SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(lf), &lf, 0);
	lf.lfHeight = -13;
	lf.lfWeight = FW_BOLD;

	HFONT hFont = CreateFontIndirect(&lf);
	SendMessage(m_hEditName, WM_SETFONT, (WPARAM)hFont, TRUE);

	for (CComponent* c : _target->getComponentList())
	{
		int num = (int)c->GetInspectorFields().size();
		int h = m_sOptinos.contstsBarHeight + num * fieldH;

		HWND box = CreateWindowEx
		(
			0, L"STATIC", nullptr,
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			x, y, 
			itemW, h,
			m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr
		);

		HWND componentTop = CreateWindowEx
		(
			0, L"STATIC", nullptr,
			WS_CHILD | WS_VISIBLE,
			x, y,
			itemW, m_sOptinos.contstsBarHeight,
			m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr
		);

		m_vChildWindows.push_back(componentTop);

		SetWindowLongPtr(componentTop, GWLP_USERDATA, static_cast<LONG_PTR>(HWND_INSPECTORCOMPONENTTOP));

		LOGFONT lf{};
		SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(lf), &lf, 0);
		lf.lfHeight = -13;      
		lf.lfWeight = FW_BOLD;      

		HFONT hFont = CreateFontIndirect(&lf);
		SendMessage(componentTop, WM_SETFONT, (WPARAM)hFont, TRUE);

		SetWindowText(componentTop, c->getName().c_str());

		m_vContentsWindows.push_back(box);
		m_iTotalHeight += h;
		y += h;
	}

	UpdateScrollInfo();

	SCROLLINFO si{ sizeof(si), SIF_RANGE | SIF_PAGE };
	GetScrollInfo(m_hWnd, SB_VERT, &si);
	const bool hasScroll = UINT(si.nMax - si.nMin + 1) > si.nPage;

	const int sbW = hasScroll ? GetSystemMetrics(SM_CXVSCROLL) : 0;
	const int finalW = clientW - sbW - margin;

	for (HWND child : m_vContentsWindows)
	{
		RECT r; GetWindowRect(child, &r);
		MapWindowPoints(HWND_DESKTOP, m_hWnd, (POINT*)&r, 2);
		MoveWindow(child, r.left, r.top, finalW, r.bottom - r.top, TRUE);
	}

	for (HWND child : m_vChildWindows)
	{
		RECT r; GetWindowRect(child, &r);
		MapWindowPoints(HWND_DESKTOP, m_hWnd, (POINT*)&r, 2);
		MoveWindow(child, r.left, r.top, finalW, r.bottom - r.top, TRUE);
	}
}

void CInspectorWindow::UpdateScrollInfo()
{
	if (m_iTotalHeight <= 0)
		return;

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
	return;
	SetScrollInfo(m_hWnd, SB_VERT, &si, TRUE);

	const BOOL needScroll = (m_iTotalHeight > viewH);
	ShowScrollBar(m_hWnd, SB_VERT, needScroll);
}
