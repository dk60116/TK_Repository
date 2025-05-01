#include "CHierachyWindow.h"
#include "CScene.h"
#include "CGameObject.h"

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "UxTheme.lib")

CHierachyWindow::CHierachyWindow()
	: m_sOptions({})
	, m_hTreeView(nullptr)
	, m_hLeftSideArea(nullptr)
	, m_hPressedItem(nullptr)
	, m_iLeftSideWidth(40)
	, m_hFont(nullptr)
{
	m_strName = L"Editor:Hierachy Window";
}

CHierachyWindow::~CHierachyWindow()
{
}

HRESULT CHierachyWindow::Init(HWND _hWnd, vector2Int _size, _bool _isBase)
{
	if (FAILED(__super::Init(_hWnd, _size, _isBase)))
		return E_FAIL;

	InitCommonControls();

	m_hTreeView = CreateWindowEx
	(
		0, WC_TREEVIEW, L"Hierachy Tree",
		WS_VISIBLE | WS_CHILD |
		TVS_SHOWSELALWAYS | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_TRACKSELECT | TVS_FULLROWSELECT | TVS_NONEVENHEIGHT,
		m_iLeftSideWidth, CHILDTOPBARHEIGHT,
		_size.x - m_iLeftSideWidth, _size.y - CHILDTOPBARHEIGHT,
		_hWnd, nullptr, GetModuleHandle(NULL), nullptr
	);

	SetWindowTheme(m_hTreeView, L"", L"");

	SetTreeViewOptions();

	SetWindowSubclass(m_hTreeView, TreeSubProc, 0, (DWORD_PTR)this);

	m_hLeftSideArea = CreateWindowEx
	(
		0, L"STATIC", nullptr,
		WS_VISIBLE | WS_CHILD,
		0, CHILDTOPBARHEIGHT,
		m_iLeftSideWidth, _size.y - CHILDTOPBARHEIGHT,
		_hWnd, nullptr, GetModuleHandle(NULL), nullptr
	);

	SetWindowLongPtr(m_hLeftSideArea, GWLP_USERDATA, static_cast<LONG_PTR>(HWND_HIERACHYSIDE));

	return S_OK;
}

void CHierachyWindow::Render()
{
}

void CHierachyWindow::Update()
{
}

void CHierachyWindow::UpdateResolution(HWND _target, vector2Int _resolution)
{
	if (m_hWnd != _target)
		return;

	__super::UpdateResolution(_target, _resolution);

	MoveWindow(m_hTreeView, m_iLeftSideWidth, CHILDTOPBARHEIGHT, _resolution.x - m_iLeftSideWidth, _resolution.y - CHILDTOPBARHEIGHT, TRUE);
}

LRESULT CHierachyWindow::WndProcHandle(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	switch (_message)
	{
	case WM_LBUTTONDOWN:
	{
		POINT pt;
		pt.x = GET_X_LPARAM(_lParam);
		pt.y = GET_Y_LPARAM(_lParam);

		TVHITTESTINFO hitTestInfo{};
		hitTestInfo.pt = pt;

		if (GetForegroundWindow() == m_hWnd && hitTestInfo.hItem == nullptr)
		{
			TreeView_SelectItem(m_hTreeView, nullptr);
			CEngineEditor::GetInstance().getWindow<CInspectorWindow>()->ViewTargetInfor_GameObject(nullptr);
		}
	}
	break;

	case WM_NOTIFY:
	{
		LPNMHDR pNMHDR = reinterpret_cast<LPNMHDR>(_lParam);

		if (pNMHDR->hwndFrom == m_hTreeView)
		{
			auto pCD = reinterpret_cast<LPNMTVCUSTOMDRAW>(_lParam);

			if (pNMHDR->code == TVN_SELCHANGED)
			{
				LPNMTREEVIEW pNMTV = reinterpret_cast<LPNMTREEVIEW>(_lParam);

				HTREEITEM hSelectedItem = pNMTV->itemNew.hItem;
				HWND treeHandle = CEngineEditor::GetInstance().getWindow<CHierachyWindow>()->getTreeHandle();

				TVITEMW tvi = {};
				tvi.hItem = hSelectedItem;
				tvi.mask = TVIF_PARAM;

				if (TreeView_GetItem(treeHandle, &tvi))
				{
					CGameObject* pSelectedObject = reinterpret_cast<CGameObject*>(tvi.lParam);

					if (dynamic_cast<CGameObject*>(pSelectedObject))
					{
						CEngineEditor::GetInstance().SelectGameObject(pSelectedObject);
					}
				}
			}
		}

		if (pNMHDR->code == NM_DBLCLK)
		{
			DWORD dwPos = GetMessagePos();

			POINT pt = { GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos) };
			ScreenToClient(m_hTreeView, &pt);

			TVHITTESTINFO hitTestInfo = {};
			hitTestInfo.pt = pt;

			TreeView_HitTest(m_hTreeView, &hitTestInfo);

			HTREEITEM hClickedItem = hitTestInfo.hItem;

			TVITEM tvi = {};
			tvi.hItem = hClickedItem;
			tvi.mask = TVIF_PARAM;

			if (TreeView_GetItem(m_hTreeView, &tvi))
			{
				CGameObject* pClickedObj = reinterpret_cast<CGameObject*>(tvi.lParam);
				if (pClickedObj)
				{
					CManagement::GetInstance().getEditorCamera().GotoViewGameObject(pClickedObj);
				}
			}

			return TRUE;
		}

		return 0;
	}

	default:
		break;
	}

	return __super::WndProcHandle(_hWnd, _message, _wParam, _lParam);
}

void CHierachyWindow::BuildTree()
{
	TreeView_DeleteAllItems(m_hTreeView);

	AddSceneRecursive(CManagement::GetInstance().getCrtScene());

	HTREEITEM hRootItem = TreeView_GetRoot(m_hTreeView);
	TreeView_Expand(m_hTreeView, hRootItem, TVE_EXPAND);

	AdjustTreeHeight();
}

void CHierachyWindow::AddSceneRecursive(CScene* _scene)
{
	TVINSERTSTRUCTW tvInsert = {};
	tvInsert.hParent = NULL;
	tvInsert.hInsertAfter = TVI_LAST;
	tvInsert.item.mask = TVIF_TEXT | TVIF_PARAM;
	tvInsert.item.pszText = const_cast<wchar_t*>(_scene->getName().c_str());

	HTREEITEM hItem = TreeView_InsertItem(m_hTreeView, &tvInsert);

	for (auto& gameObject : CManagement::GetInstance().getCrtScene()->getRootObjects())
		AddGameObjectRecursive(hItem, gameObject);
}

void CHierachyWindow::AddGameObjectRecursive(HTREEITEM _parentItem, CGameObject* _gameObject)
{
	TVINSERTSTRUCTW tvInsert = {};
	tvInsert.hParent = _parentItem;
	tvInsert.hInsertAfter = TVI_LAST;
	tvInsert.item.mask = TVIF_TEXT | TVIF_PARAM;
	tvInsert.item.pszText = const_cast<wchar_t*>(_gameObject->getName().c_str());
	tvInsert.item.lParam = reinterpret_cast<LPARAM>(_gameObject);

	HTREEITEM hItem = TreeView_InsertItem(m_hTreeView, &tvInsert);

	for (auto& child : _gameObject->getTransform().getChilds())
		AddGameObjectRecursive(hItem, child->getObject());
}

void CHierachyWindow::SetTreeViewOptions()
{
	if (m_hFont)
		DeleteObject(m_hFont);

	m_hFont = CreateFontW
	(
		-m_sOptions.fontSize, 0, 0, 0,
		FW_NORMAL, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"Segoe UI"
	);

	SendMessage(m_hTreeView, WM_SETFONT, (WPARAM)m_hFont, TRUE);

	TreeView_SetBkColor(m_hTreeView, CEngineEditor::GetInstance().getOptions().s_baseColor.rColor());
	TreeView_SetTextColor(m_hTreeView, m_sOptions.textColor.rColor());
}

const _bool CHierachyWindow::IsTreeViewHighlightEmpty()
{
	auto a = TreeView_GetSelection(m_hTreeView);
	if (TreeView_GetSelection(m_hTreeView) == NULL) 
		return false;
	if (TreeView_GetHotItem(m_hTreeView) == NULL)
		return false;
	if ((HTREEITEM)SendMessage
	(
		m_hTreeView, TVM_GETNEXTITEM, TVGN_DROPHILITE, 0) != NULL
	)
		return false;  

	return true;
}

LRESULT CHierachyWindow::TreeSubProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam, UINT_PTR _idSubClass, DWORD_PTR _dwRefData)
{
	auto* self = reinterpret_cast<CHierachyWindow*>(_dwRefData);

    switch (_msg)
    {
	case WM_LBUTTONDOWN:
	{
		POINT pt;
		pt.x = GET_X_LPARAM(_lParam);
		pt.y = GET_Y_LPARAM(_lParam);

		TVHITTESTINFO hitTestInfo{};
		hitTestInfo.pt = pt;

		if (hitTestInfo.hItem == nullptr)
		{
			TreeView_SelectItem(self->m_hTreeView, nullptr);
		}
	}
	break;

	case WM_LBUTTONUP:
	{
	}
	break;

    case WM_CAPTURECHANGED:   // 드래그 도중 ESC 또는 외부 클릭 등
        break;
    }

    return DefSubclassProc(_hWnd, _msg, _wParam, _lParam);
}

bool CHierachyWindow::GetLastVisibleItemRect(RECT& _rcOut)
{
	HTREEITEM hLast = (HTREEITEM)SendMessage(
		m_hTreeView, TVM_GETNEXTITEM, TVGN_LASTVISIBLE, 0);

	if (!hLast) return false;

	if (!TreeView_GetItemRect(m_hTreeView, hLast, &_rcOut, TRUE))
		return false;

	return true;
}

void CHierachyWindow::AdjustTreeHeight()
{
	RECT rc{};  

	if (!GetLastVisibleItemRect(rc))
		return;
	
	RECT rcTree;  GetWindowRect(m_hTreeView, &rcTree);
	int newH = rc.bottom;                       // 컨텐츠 끝 + 패딩
	int newW = rcTree.right - rcTree.left;

	// 클라이언트 좌표로 변환
	POINT pt = { 0,0 }; ClientToScreen(m_hWnd, &pt);
	newH = max(newH, 4);                        // 최소 높이 safeguard

	MoveWindow
	(
		m_hTreeView,
		m_iLeftSideWidth, CHILDTOPBARHEIGHT,
		newW, newH, TRUE
	);
}
