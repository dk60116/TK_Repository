#include "CSceneWindow.h"
#include "EngineEditor.h"

CSceneWindow::CSceneWindow()
{
	m_strName = L"Editor:CScene Window";
}

CSceneWindow::~CSceneWindow()
{
}

HRESULT CSceneWindow::Init(HWND _hWnd, vector2Int _size, _bool _isBase)
{
	if (FAILED(__super::Init(_hWnd, _size, _isBase)))
		return E_FAIL;

	HINSTANCE hInstance = CEngineEditor::GetInstance().getHInstance();

	HFONT hFont14 = CEngineEditor::CreateDefaultFont(L"Arial", 18);
	HFONT hFont16 = CEngineEditor::CreateDefaultFont(L"Arial", 16, TRUE);

	HWND ee = CreateWindowW(L"COMBOBOX", L"Handle",
		WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
		10, 3, 100, 200,
		m_hWnd, NULL, hInstance, nullptr);

	SendMessageW(ee, WM_SETFONT, (WPARAM)hFont16, TRUE);

	SendMessageW(ee, CB_ADDSTRING, 0, (LPARAM)L"Center");
	SendMessageW(ee, CB_ADDSTRING, 1, (LPARAM)L"Pivot");

	SendMessageW(ee, CB_SETCURSEL, 0, 0);

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
