#include "CHierachyWindow.h"

#pragma comment(lib, "comctl32.lib")

CHierachyWindow::CHierachyWindow()
	: m_hTreeView(nullptr)
	, m_iLeftSideWidth(40)
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
		0, WC_TREEVIEW, nullptr,
		WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
		m_iLeftSideWidth, CHILDTOPBARHEIGHT,
		_size.x - m_iLeftSideWidth, _size.y - CHILDTOPBARHEIGHT,
		_hWnd, nullptr, GetModuleHandle(NULL), nullptr
	);

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
