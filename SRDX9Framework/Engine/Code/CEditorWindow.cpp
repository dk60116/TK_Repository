#include "CEditorWindow.h"

CEditorWindow::CEditorWindow()
	: m_hWnd(nullptr)
	, m_hTopBar(nullptr)
	, m_v2Resolution(vector2Int::zero())
{
}

CEditorWindow::~CEditorWindow()
{
}

void CEditorWindow::Init(HWND _hWnd, vector2Int _size)
{
	m_hWnd = _hWnd;
	m_v2Resolution = _size;
}

void CEditorWindow::UpdateResolution(vector2Int _resolutoin)
{
}
