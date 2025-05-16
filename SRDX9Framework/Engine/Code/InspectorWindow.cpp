#include "InspectorWindow.h"
#include "EngineEditor.h"
#include "Resource.h"
#include "MeshRenderer.h"
#include "SpriteRenderer.h"
#include "AudioSource.h"

CInspectorWindow::CInspectorWindow()
	: m_sOptinos({})
	, m_hEditName(nullptr)
	, m_hAddComponentBtn(nullptr)
	, m_vContentsWindows({})
	, m_vChildWindows({})
	, m_pViewGameObject(nullptr)
	, m_iTotalHeight(0)
	, m_iScrollPos(0)
	, m_hDarkBrush(CreateSolidBrush(RGB(55,55, 58)))
	, m_vPairViewList({})
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
	for (TRAVERSAL_ITER(m_vPairViewList, it))
	{
		switch ((*it).type)
		{
		case FieldType::FLOAT:
		case FieldType::FLOAT_RX:
		case FieldType::FLOAT_RY:
		case FieldType::FLOAT_RZ:
		{
			_float value = *(reinterpret_cast<_float*>((*it).value));
			_float prevValue = *(reinterpret_cast<_float*>((*it).prevValue));

			if (prevValue != value)
				SetWindowTextW((*it).handle, to_wstring(value).c_str());

			*reinterpret_cast<_float*>((*it).prevValue) = value;
		}
		break;
		default:
			break;
		}
	}
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
			SendMessage(_hWnd, WM_VSCROLL, MAKEWPARAM(steps > 0 ? SB_LINEUP : SB_LINEDOWN, 0), 0);
		return 0;
	}
	break;

	case WM_COMMAND:
	{
		HWND hCtrl = (HWND)_lParam;
		_int code = HIWORD(_wParam);

		if (code == EN_KILLFOCUS)
		{
			for (auto& pair : m_vPairViewList)
			{
				if (pair.handle == hCtrl)
				{
					wchar_t buf[64];
					GetWindowTextW(hCtrl, buf, 64);

					switch (pair.type)
					{
					case FieldType::FLOAT:
					case FieldType::FLOAT_RX:
					case FieldType::FLOAT_RY:
					case FieldType::FLOAT_RZ:
					{
						_float newValue = static_cast<_float>(_wtof(buf));

						_float* target = reinterpret_cast<_float*>(pair.value);

						if (*target != newValue)
							*target = newValue;

						CEngineEditor::GetInstance().getSelectedGameObject()->getTransform().EditRotation(pair.type, newValue);

						CManagement::GetInstance().getCrtScene()->UpdateEditor();
					}
					break;
					case FieldType::STRING:
						break;
					default:
						break;
					}
				}
			}
		}
		else if (code == BN_CLICKED)
		{
			HWND hWnd = (HWND)_lParam;

			if (GetProp(hWnd, L"CHECKBOX"))
			{
				_bool checked = (_bool)static_cast<LONG>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

				SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)(!checked));

				InvalidateRect(hWnd, nullptr, TRUE);

				for (auto& pair : m_vPairViewList)
				{
					if (pair.handle == hWnd && pair.type == FieldType::BOOL)
					{
						*reinterpret_cast<_bool*>(pair.value) = !checked;
						CManagement::GetInstance().getCrtScene()->UpdateEditor();
						break;
					}
				}
			}
			else if (GetProp(hWnd, L"ADDCOMPONENTBUTTON"))
			{
				SendMessage(hWnd, BM_SETSTATE, FALSE, 0); 
				SetFocus(NULL);                            
				ReleaseCapture();                          

				ShowAddComponentMenu();
			}
		}
	}
	break;

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT dis = (LPDRAWITEMSTRUCT)_lParam;

		HWND hWnd = dis->hwndItem;

		if (GetProp(hWnd, L"CHECKBOX"))
		{
			LONG_PTR tag = GetWindowLongPtr(hWnd, GWLP_USERDATA);

			_bool checked = (_bool)static_cast<LONG>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

			HDC hdc = dis->hDC;
			RECT rc = dis->rcItem;

			FillRect(hdc, &rc, m_hDarkBrush);

			const int radius = 6;
			HBRUSH frame = CreateSolidBrush(RGB(0, 0, 0));
			HPEN   pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			HGDIOBJ oldPen = SelectObject(hdc, pen);
			HGDIOBJ oldBrush = SelectObject(hdc, frame);
			RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, radius, radius);

			RECT inner = rc;
			InflateRect(&inner, -1, -1);

			HBRUSH fill = CreateSolidBrush(CEngineEditor::GetInstance().getOptions().inspectorBoxColor.rColor());
			FillRect(hdc, &inner, fill);
			DeleteObject(fill);

			if (checked)
			{
				HPEN checkPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
				HGDIOBJ oldPen = SelectObject(hdc, checkPen);

				int cx = (rc.left + rc.right) / 2;
				int cy = (rc.top + rc.bottom) / 2;
				int size = min(rc.right - rc.left, rc.bottom - rc.top) / 3;

				// 체크 모양: 왼쪽 아래 → 가운데 → 오른쪽 위
				MoveToEx(hdc, cx - size, cy, NULL);
				LineTo(hdc, cx, cy + size / 2);
				LineTo(hdc, cx + size, cy - size);

				SelectObject(hdc, oldPen);
				DeleteObject(checkPen);
			}

			SelectObject(hdc, oldPen);
			SelectObject(hdc, oldBrush);
			DeleteObject(pen);
			DeleteObject(frame);
		}
		else if (GetProp(hWnd, L"ADDCOMPONENTBUTTON"))
		{
			HDC hdc = dis->hDC;
			RECT rc = dis->rcItem;

			// 배경색
			COLORREF bgColor = RGB(88, 88, 88);
			HBRUSH bg = CreateSolidBrush(bgColor);
			FillRect(hdc, &rc, bg);
			DeleteObject(bg);

			// 라운드 테두리
			HPEN pen = CreatePen(PS_SOLID, 1, RGB(30, 30, 30));
			HGDIOBJ oldPen = SelectObject(hdc, pen);
			HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
			RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 10, 10);
			SelectObject(hdc, oldBrush);
			SelectObject(hdc, oldPen);
			DeleteObject(pen);

			// 텍스트
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(255, 255, 255));

			wstring text = L"Add Component";
			DrawTextW(hdc, text.c_str(), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			return TRUE;
		}

		return TRUE;
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
				break;
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

	for (TRAVERSAL_ITER(m_vPairViewList, it))
	{
		Safe_Delete((*it).prevValue);
	}

	m_vPairViewList.clear();
}

void CInspectorWindow::ClearComponents()
{
	if (IsWindow(m_hEditName))
		DestroyWindow(m_hEditName);

	if (IsWindow(m_hAddComponentBtn))
		DestroyWindow(m_hAddComponentBtn);

	m_hEditName = nullptr;
	m_hAddComponentBtn = nullptr;

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

	for (TRAVERSAL_ITER(m_vPairViewList, it))
	{
		Safe_Delete((*it).prevValue);
	}

	m_vContentsWindows.clear();
	m_vChildWindows.clear();
	m_vPairViewList.clear();

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

	HWND top = CreateWindowEx
	(
		0, L"STATIC", nullptr,
		WS_CHILD | WS_VISIBLE,
		x, y, 
		itemW, m_sOptinos.topHeight,
		m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr
	);

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

		HWND componentTitle = CreateWindowEx
		(
			0, L"STATIC", nullptr,
			WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE | SS_LEFT,
			x + 30, y,
			itemW - 30, m_sOptinos.contstsBarHeight,
			m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr
		);

		m_vChildWindows.push_back(componentTitle);

		SetWindowLongPtr(componentTitle, GWLP_USERDATA, static_cast<LONG_PTR>(HWND_INSPECTORCOMPONENTTOP));

		LOGFONT lf{};
		SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(lf), &lf, 0);
		lf.lfHeight = -13;      
		lf.lfWeight = FW_BOLD;      

		HFONT hFont = CreateFontIndirect(&lf);
	
		wstring name = c->getName();

		SetWindowText(componentTitle, name.c_str());

		for (int i = 0; i < c->GetInspectorFields().size(); ++i)
		{
			vector2Int defaultPos = vector2Int(x, m_sOptinos.contstsBarHeight + y + (25 * i) - 2);
			vector2Int defaultSize = vector2Int(itemW, 30);
			wstring defaultName = c->GetInspectorFields()[i].name;

			switch (c->GetInspectorFields()[i].type)
			{
			case FieldType::FLOAT:
				CreateFloatBox(defaultPos, defaultSize, defaultName, static_cast<_float*>(c->GetInspectorFields()[i].ptr));
				break;
			case FieldType::BOOL:
				CreateBoolBox(defaultPos, defaultSize, defaultName, static_cast<_bool*>(c->GetInspectorFields()[i].ptr));
				break;
			case FieldType::VECTOR3:
				CreateVector3Box(defaultPos, defaultSize, defaultName, static_cast<vector3*>(c->GetInspectorFields()[i].ptr));
				break;
			case FieldType::POINTER:
				CreatePointerBox(defaultPos, defaultSize, defaultName, static_cast<UObject**>(c->GetInspectorFields()[i].ptr));
				break;
			case FieldType::STRING:
				break;
			default:
				break;
			}
		}

		m_vContentsWindows.push_back(box);
		m_iTotalHeight += h;
		y += h;
	}

	m_hAddComponentBtn = CreateWindowEx
	(
		0, L"BUTTON", nullptr,
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		static_cast<_int>(x + itemW * 0.15f), m_iTotalHeight + 10,
		static_cast<_int>(itemW * 0.7f), 25,
		m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr
	);

	m_iTotalHeight += 45;

	SetProp(m_hAddComponentBtn, L"ADDCOMPONENTBUTTON", (HANDLE)TRUE);

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
}

void CInspectorWindow::ShowAddComponentMenu()
{
	HMENU hPopupMenu = CreatePopupMenu();

	AppendMenu(hPopupMenu, MF_STRING, 1001, L"Camera");
	AppendMenu(hPopupMenu, MF_STRING, 1002, L"Light");
	AppendMenu(hPopupMenu, MF_STRING, 1101, L"Mesh Filter");
	AppendMenu(hPopupMenu, MF_STRING, 1102, L"Mesh Renderer");
	AppendMenu(hPopupMenu, MF_STRING, 1103, L"Sprite Renderer");
	AppendMenu(hPopupMenu, MF_STRING, 1201, L"Audio Source");

	POINT pt;
	GetCursorPos(&pt);

	int selected = TrackPopupMenu
	(
		hPopupMenu,
		TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD,
		pt.x, pt.y, 0,
		m_hWnd, NULL
	);

	DestroyMenu(hPopupMenu);

	if (selected != 0)
	{
		if (FAILED(HandleComponentAdd(selected)))
			return;
	}
}

HRESULT CInspectorWindow::HandleComponentAdd(_int _id)
{
	CGameObject* target = CEngineEditor::GetInstance().getSelectedGameObject();
	
	if (!target)
		return E_FAIL;

	switch (_id)
	{
	case 1001:
		target->AddComponent<CCamera>();
		break;
	case 1002:
		target->AddComponent<CLight>();
		break;
	case 1101:
		target->AddComponent<CMeshFilter>();
		break;
	case 1102:
		target->AddComponent<CMeshRenderer>();
		break;
	case 1103:
		target->AddComponent<CSpriteRenderer>();
		break;
	case 1201:
		target->AddComponent<CAudioSource>();
		break;
	default:
		break;
	}

	CManagement::GetInstance().getCrtScene()->UpdateEditor();
	ViewTargetInfor_GameObject(target);

	return S_OK;
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

	const BOOL needScroll = (m_iTotalHeight > viewH);
	ShowScrollBar(m_hWnd, SB_VERT, needScroll);
}

#pragma region boolBox
void CInspectorWindow::CreateBoolBox(vector2Int _start, vector2Int _size, wstring _name, _bool* _value)
{
	HWND boolView = CreateWindowEx
	(
		0, L"STATIC", nullptr,
		WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE | SS_LEFT,
		_start.x, _start.y,
		_size.x, _size.y,
		m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr
	);

	m_vChildWindows.push_back(boolView);

	SetWindowLongPtr(boolView, GWLP_USERDATA, static_cast<LONG_PTR>(HWND_INSPECTORCOMPONENTBODY));

	SetWindowText(boolView, (L"     " + _name).c_str());

	_int leftOffset = 100 + static_cast<_int>(_size.x * 0.025f);
	_int rightOffset = 5;
	_int rightArea = _size.x - leftOffset;
	_int x = _start.x + leftOffset;
	_int y = _start.y + 4;
	_int width = _size.y - 10;
	_int height = _size.y - 10;

	HWND checkBox = CreateWindowEx
	(
		0, L"BUTTON", nullptr,
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		x + static_cast<_int>(width * 0.5f), y,
		width, height,
		m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr	
	);

	SetWindowTheme(checkBox, L"", L"");

	m_vChildWindows.push_back(checkBox);

	m_vPairViewList.push_back({ checkBox, _value, new _bool(), FieldType::BOOL });

	SetProp(checkBox, L"CHECKBOX", (HANDLE)TRUE);
	SetWindowLongPtr(checkBox, GWLP_USERDATA, (LONG_PTR)(*_value));
}
#pragma endregion

#pragma region floatBox
void CInspectorWindow::CreateFloatBox(vector2Int _start, vector2Int _size, wstring _name, _float* _value)
{
	HWND floatView = CreateWindowEx
	(
		0, L"STATIC", nullptr,
		WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE | SS_LEFT,
		_start.x, _start.y,
		_size.x, _size.y,
		m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr
	);

	m_vChildWindows.push_back(floatView);

	SetWindowLongPtr(floatView, GWLP_USERDATA, static_cast<LONG_PTR>(HWND_INSPECTORCOMPONENTBODY));

	SetWindowText(floatView, (L"     " + _name).c_str());

	_int leftOffset = 110 + static_cast<_int>(_size.x * 0.025f);
	_int rightOffset = 5;
	_int rightArea = _size.x - leftOffset;
	_int x = _start.x + leftOffset;
	_int y = _start.y + 4;
	_int width = _int((_size.x - leftOffset) * 0.25f);
	_int height = _size.y - 10;

	HWND inputBox = CreateWindowEx
	(
		0, L"Edit", nullptr,
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_BORDER,
		x, y,
		width, height,
		m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr
	);

	SetWindowTextW(inputBox, to_wstring(*_value).c_str());

	m_vChildWindows.push_back(inputBox);

	m_vPairViewList.push_back({ inputBox, _value, new _float(), FieldType::FLOAT });

	SetWindowLongPtr(inputBox, GWLP_USERDATA, static_cast<LONG_PTR>(HWND_INPUTBOX));
}
#pragma endregion

#pragma region vector3Box
void CInspectorWindow::CreateVector3Box(vector2Int _start, vector2Int _size, wstring _name, vector3* _value)
{
	HWND vector3View = CreateWindowEx
	(
		0, L"STATIC", nullptr,
		WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE | SS_LEFT,
		_start.x, _start.y,
		_size.x, _size.y,
		m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr
	);

	m_vChildWindows.push_back(vector3View);

	SetWindowLongPtr(vector3View, GWLP_USERDATA, static_cast<LONG_PTR>(HWND_INSPECTORCOMPONENTBODY));

	SetWindowText(vector3View, (L"     " + _name).c_str());

	_int leftOffset = 110 + static_cast<_int>(_size.x * 0.025f);
	_int rightOffset = 5;
	_int rightArea = _size.x - leftOffset;
	_int x = _start.x + leftOffset;
	_int y = _start.y + 4;
	_int width = _int((_size.x - leftOffset) * 0.25f);
	_int height = _size.y - 10;

	HWND xView = CreateWindowEx
	(
		0, L"Edit", nullptr,
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_BORDER,
		x, y,
		width, height,
		m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr
	);

	SetWindowTextW(xView, to_wstring((*_value).x).c_str());

	HWND yView = CreateWindowEx
	(
		0, L"Edit", nullptr,
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_BORDER,
		_int(_size.x - (rightArea * 0.5f) - (width * 0.5f)), y,
		width, height,
		m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr
	);

	SetWindowTextW(yView, to_wstring((*_value).y).c_str());

	HWND zView = CreateWindowEx
	(
		0, L"Edit", nullptr,
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_BORDER,
		_size.x - width - rightOffset, y,
		width, height,
		m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr
	);

	SetWindowTextW(zView, to_wstring((*_value).z).c_str());

	if (_name == L"Rotation")
	{
		m_vPairViewList.push_back({ xView, &(_value->x), new _float(), FieldType::FLOAT_RX });
		m_vPairViewList.push_back({ yView, &(_value->y), new _float(), FieldType::FLOAT_RY });
		m_vPairViewList.push_back({ zView, &(_value->z), new _float(), FieldType::FLOAT_RZ });
	}
	else
	{
		m_vPairViewList.push_back({ xView, &(_value->x), new _float(), FieldType::FLOAT });
		m_vPairViewList.push_back({ yView, &(_value->y), new _float(), FieldType::FLOAT });
		m_vPairViewList.push_back({ zView, &(_value->z), new _float(), FieldType::FLOAT });
	}

	m_vChildWindows.push_back(xView);
	m_vChildWindows.push_back(yView);
	m_vChildWindows.push_back(zView);

	SetWindowLongPtr(xView, GWLP_USERDATA, static_cast<LONG_PTR>(HWND_INPUTBOX));
	SetWindowLongPtr(yView, GWLP_USERDATA, static_cast<LONG_PTR>(HWND_INPUTBOX));
	SetWindowLongPtr(zView, GWLP_USERDATA, static_cast<LONG_PTR>(HWND_INPUTBOX));
}
#pragma endregion

#pragma region pointerBox
void CInspectorWindow::CreatePointerBox(vector2Int _start, vector2Int _size, wstring _name, UObject** _value)
{
	HWND pointerView = CreateWindowEx
	(
		0, L"STATIC", nullptr,
		WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE | SS_LEFT,
		_start.x, _start.y,
		_size.x, _size.y,
		m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr
	);

	m_vChildWindows.push_back(pointerView);

	SetWindowText(pointerView, (L"     " + _name).c_str());

	_int leftOffset = 110 + static_cast<_int>(_size.x * 0.025f);
	_int rightOffset = 5;
	_int rightArea = _size.x - leftOffset;
	_int x = _start.x + leftOffset;
	_int y = _start.y + 4;
	_int width = rightArea - rightOffset;
	_int height = _size.y - 10;

	HWND nameView = CreateWindowEx
	(
		0, L"Edit", nullptr,
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_BORDER,
		x, y,
		width, height,
		m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr
	);

	m_vChildWindows.push_back(nameView);

	SetWindowLongPtr(nameView, GWLP_USERDATA, static_cast<LONG_PTR>(HWND_INPUTBOX));

	if (CComponent* cp = dynamic_cast<CComponent*>(*_value))
	{
		wstring rout = cp->getObject()->getName() + wstring(L" ") + wstring(L"(") + cp->getName() + wstring(L")");
		SetWindowTextW(nameView, rout.c_str());
	}
	else if (CResource* rs = dynamic_cast<CResource*>(*_value))
	{
		SetWindowTextW(nameView, rs->getName().c_str());
	}
	else
		SetWindowTextW(nameView, L"nullptr");

	m_vPairViewList.push_back({ nameView, _value, nullptr, FieldType::STRING });

	SetWindowLongPtr(pointerView, GWLP_USERDATA, static_cast<LONG_PTR>(HWND_INSPECTORCOMPONENTBODY));
}
#pragma endregion
