#include "CGameWindow.h"

CGameWindow::CGameWindow()
{
	m_strName = L"Scene Window";
}

CGameWindow::~CGameWindow()
{
}

HRESULT CGameWindow::Init(HWND _hWnd, vector2Int _size)
{
	if (FAILED(__super::Init(_hWnd, _size)))
		return E_FAIL;

	return S_OK;
}

void CGameWindow::Render()
{
}

void CGameWindow::Update()
{
}

void CGameWindow::UpdateResolution(HWND _target, vector2Int _resolution)
{
	if (m_hWnd != _target)
		return;

	__super::UpdateResolution(_target, _resolution);
}
