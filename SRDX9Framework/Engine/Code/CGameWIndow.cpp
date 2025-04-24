#include "CGameWIndow.h"

CGameWIndow::CGameWIndow()
{
	m_strName = L"Scene Window";
}

CGameWIndow::~CGameWIndow()
{
}

HRESULT CGameWIndow::Init(HWND _hWnd, vector2Int _size)
{
	if (FAILED(__super::Init(_hWnd, _size)))
		return E_FAIL;

	return S_OK;
}

void CGameWIndow::Render()
{
}

void CGameWIndow::Update()
{
}

void CGameWIndow::UpdateResolution(HWND _target, vector2Int _resolution)
{
	if (m_hWnd != _target)
		return;

	__super::UpdateResolution(_target, _resolution);
}
