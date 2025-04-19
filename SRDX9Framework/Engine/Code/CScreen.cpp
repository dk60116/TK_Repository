#include "CScreen.h"

CScreen::CScreen()
	: m_hInstance(nullptr)
	, m_hMainWnd(nullptr)
	, m_hSceneWnd(nullptr)
	, m_hGameWnd(nullptr)
	, m_v2Resolution(vector2(960, 540))
	, m_bFullScreen(false)
	, m_iDPI(0)
{
}

CScreen::~CScreen()
{
}

void CScreen::Start_Window(LPWCH _winClass, HINSTANCE _hInst, int _cmdShow)
{
	m_hInstance = _hInst;

#ifdef _DEBUG
	RECT rc{ 0, 0, CScreen::GetInstance().getResolution().x, CScreen::GetInstance().getResolution().y * 2 };
#else
	RECT rc{ 0, 0, CScreen::GetInstance().getResolution().x, CScreen::GetInstance().getResolution().y };
#endif

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	RECT crc = { 0, 0, CScreen::GetInstance().getResolution().x, CScreen::GetInstance().getResolution().y };
	AdjustWindowRect(&crc, WS_OVERLAPPEDWINDOW, FALSE);

	int offset = 0;

#ifdef _DEBUG
	offset = 30;

	m_hMainWnd = CreateWindowW(_winClass, L"Main",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		offset, rc.right - rc.left, rc.bottom - rc.top,
		nullptr, nullptr, _hInst, nullptr);
#endif

	m_hGameWnd = CreateWindowW(_winClass, L"Game",
		WS_OVERLAPPEDWINDOW, GW_CHILD,
		0, crc.right - crc.left, crc.bottom - crc.top,
		m_hMainWnd, nullptr, _hInst, nullptr);

	ShowWindow(m_hMainWnd, _cmdShow);
	ShowWindow(m_hGameWnd, _cmdShow);

	UpdateWindow(m_hMainWnd);
	UpdateWindow(m_hGameWnd);
}

void CScreen::UpdateResolution(const _int& _width, const _int& _height)
{
	m_v2Resolution = vector2Int(_width, _height);
}
