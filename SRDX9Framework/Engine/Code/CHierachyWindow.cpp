#include "CHierachyWindow.h"

#pragma comment(lib, "comctl32.lib")

CHierachyWindow::CHierachyWindow()
	: m_hTreeView(nullptr)
{
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
		40, CHILDTOPBARHEIGHT, _size.x, _size.y,
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
}
