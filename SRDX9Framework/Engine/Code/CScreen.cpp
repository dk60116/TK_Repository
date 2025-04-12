#include "CScreen.h"

CScreen::CScreen()
	: m_hInstance(nullptr)
	, m_hMainWnd(nullptr)
	, m_hGameWnd(nullptr)
	, m_v2Resolution(vector2(960, 540))
	, m_bFullScreen(false)
	, m_iDPI(0)
{
}

CScreen::~CScreen()
{
}

void CScreen::Start_Window(HINSTANCE _hInst, HWND _hMainWnd, HWND _hGameWnd)
{
	m_hInstance = _hInst;
	m_hMainWnd = _hMainWnd;
	m_hGameWnd = _hGameWnd;
}

void CScreen::UpdateResolution(const _int& _width, const _int& _height)
{
	m_v2Resolution = vector2Int(_width, _height);
}
