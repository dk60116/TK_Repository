#include "CScreen.h"

CScreen::CScreen()
	: m_hGameWnd(nullptr)
	, m_v2Resolution(vector2(800, 600))
	, m_bFullScreen(false)
	, m_iDPI(0)
{
}

CScreen::~CScreen()
{
}

void CScreen::Start_Window(HWND _wnd)
{
	m_hGameWnd = _wnd;
}
