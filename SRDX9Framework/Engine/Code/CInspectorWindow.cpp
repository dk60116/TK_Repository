#include "CInspectorWindow.h"

CInspectorWindow::CInspectorWindow()
{
	m_strName = L"Editor:Inspector Window";
}

CInspectorWindow::~CInspectorWindow()
{
}

HRESULT CInspectorWindow::Init(HWND _hWnd, vector2Int _size, _bool _isBase)
{
	__super::Init(_hWnd, _size, _isBase);

	return S_OK;
}

void CInspectorWindow::Render()
{
}

void CInspectorWindow::Update()
{
}

void CInspectorWindow::UpdateResolution(HWND _target, vector2Int _resolution)
{
}
