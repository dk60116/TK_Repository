#include "CBaseWindow.h"

CBaseWindow::CBaseWindow(HWND _hWnd)
	: m_hPlayBtn(nullptr)
	, m_hPauseBtn(nullptr)
	, m_hOneFrameBtn(nullptr)
{
	m_hWnd = _hWnd;
}

CBaseWindow::~CBaseWindow()
{
}

void CBaseWindow::Init(HWND _hWnd, vector2Int _size)
{
	__super::Init(_hWnd, _size);
}

void CBaseWindow::Render()
{
}

void CBaseWindow::Update()
{
}

void CBaseWindow::UpdateResolution(vector2Int _resolutoin)
{
}
