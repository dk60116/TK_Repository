#include "CSceneWindow.h"

CSceneWindow::CSceneWindow()
{
	m_strName = L"CScene Window";
}

CSceneWindow::~CSceneWindow()
{
}

HRESULT CSceneWindow::Init(HWND _hWnd, vector2Int _size)
{
	if (FAILED(__super::Init(_hWnd, _size)))
		return E_FAIL;

	return S_OK;
}

void CSceneWindow::Render()
{
}

void CSceneWindow::Update()
{
}

void CSceneWindow::UpdateResolution(HWND _target, vector2Int _resolution)
{
	if (m_hWnd != _target)
		return;

	__super::UpdateResolution(_target, _resolution);
}
