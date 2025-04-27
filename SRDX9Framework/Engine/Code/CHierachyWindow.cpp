#include "CHierachyWindow.h"
#include "CScene.h"
#include "CGameObject.h"

#pragma comment(lib, "comctl32.lib")

CHierachyWindow::CHierachyWindow()
	: m_sOptions({})
	, m_hTreeView(nullptr)
	, m_iLeftSideWidth(40)
	, m_hFont(nullptr)
{
	m_strName = L"Hierachy Window";
}

CHierachyWindow::~CHierachyWindow()
{
}

HRESULT CHierachyWindow::Init(HWND _hWnd, vector2Int _size)
{
	if (FAILED(__super::Init(_hWnd, _size)))
		return E_FAIL;

	InitCommonControls();

	m_hTreeView = CreateWindowEx
	(
		0, WC_TREEVIEW, L"Hierachy Tree",
		WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_TRACKSELECT | TVS_FULLROWSELECT | TVS_NONEVENHEIGHT,
		m_iLeftSideWidth, CHILDTOPBARHEIGHT,
		_size.x - m_iLeftSideWidth, _size.y - CHILDTOPBARHEIGHT,
		_hWnd, nullptr, GetModuleHandle(NULL), nullptr
	);
	
	TreeView_SetExtendedStyle(m_hTreeView,
		TVS_EX_DOUBLEBUFFER,
		TVS_EX_DOUBLEBUFFER | TVS_EX_FADEINOUTEXPANDOS);

	SetTreeViewOptions();

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
	case WM_NOTIFY:
	{
		LPNMHDR pNMHDR = reinterpret_cast<LPNMHDR>(_lParam);

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

				if (pSelectedObject)
				{
					CEngineEditor::GetInstance().SelectGameObject(pSelectedObject);
				}
			}
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
}

void CHierachyWindow::AddSceneRecursive(CScene* _scene)
{
	TVINSERTSTRUCTW tvInsert = {};
	tvInsert.hParent = NULL;
	tvInsert.hInsertAfter = TVI_LAST;
	tvInsert.item.mask = TVIF_TEXT | TVIF_PARAM;
	tvInsert.item.pszText = const_cast<wchar_t*>(_scene->getName().c_str());
	tvInsert.item.lParam = reinterpret_cast<LPARAM>(_scene);

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
