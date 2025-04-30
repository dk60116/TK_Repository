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
	return E_NOTIMPL;
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
