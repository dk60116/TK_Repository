#include "CHierachyWindow.h"

#pragma comment(lib, "comctl32.lib")

CHierachyWindow::CHierachyWindow(HWND _hWnd)
	: m_hTreeView(nullptr)
{
	m_hWnd = _hWnd;
}

CHierachyWindow::~CHierachyWindow()
{
}

void CHierachyWindow::Init(HWND _hWnd, vector2Int _size)
{
	__super::Init(_hWnd, _size);

	InitCommonControls();

	m_hTreeView = CreateWindowEx
	(
		0, WC_TREEVIEW, nullptr,
		WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
		30, 0, _size.x, _size.y,
		_hWnd, nullptr, GetModuleHandle(NULL), nullptr
	);
}

void CHierachyWindow::Render()
{
}

void CHierachyWindow::Update()
{
}

void CHierachyWindow::UpdateResolution(vector2Int UpdateResolution)
{
	__super::UpdateResolution(UpdateResolution);
}
