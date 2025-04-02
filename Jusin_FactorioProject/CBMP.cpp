#include "CBMP.h"
#include "Jusin_FactorioProject.h"

extern HWND g_hWnd;

CBMP::CBMP()
	: m_hMemDC(0)
	, m_hBitmap(0)
	, m_hOldbmp(0)
{
}

CBMP::~CBMP()
{
	Release();
}

void CBMP::Load_Bmp(const TCHAR* _filePath)
{
	HDC	hDC = GetDC(g_hWnd);

	m_hMemDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hWnd, hDC);

	m_hBitmap = (HBITMAP)LoadImage(NULL,
		_filePath,
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	m_hOldbmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
}

void CBMP::Release()
{
	SelectObject(m_hMemDC, m_hOldbmp);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
