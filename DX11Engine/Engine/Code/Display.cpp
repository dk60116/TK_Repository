#include "epch.h"
#include "Display.h"

CDisplay::CDisplay()
	: m_hInst(nullptr)
	, m_hGameWindow(nullptr)
	, m_hEditorWindow(nullptr)
	, m_bIsFullScreen(false)
	, m_iWidth(1280)
	, m_iHeight(720)
{
}

CDisplay::~CDisplay()
{
	Release();
}

CDisplay& CDisplay::GetInstance()
{
	static CDisplay inst;
	return inst;
}

HRESULT CDisplay::Initialize(HINSTANCE _hInst, HWND _hGameWnd, HWND _hEditorWnd)
{
	if (!_hInst)
		return E_FAIL;
	if (!_hGameWnd)
		return E_FAIL;

	GetInstance().m_hInst = _hInst;
	GetInstance().m_hGameWindow = _hGameWnd;
	GetInstance().m_hEditorWindow = _hEditorWnd;

	RECT rc;
	GetClientRect(GetInstance().m_hGameWindow, &rc);

	return S_OK;
}

void CDisplay::Release()
{
}

HINSTANCE CDisplay::Get_HInstance()
{
	return GetInstance().m_hInst;
}

HWND CDisplay::Get_GameWindow()
{
	return GetInstance().m_hGameWindow;
}

HWND CDisplay::Get_EditorWindow()
{
	return GetInstance().m_hEditorWindow;
}

vector2Int CDisplay::Get_ScreenResolution()
{
	return vector2Int(GetInstance().m_iWidth, GetInstance().m_iHeight);
}

_float CDisplay::Get_Aspect()
{
	return static_cast<_float>(GetInstance().m_iWidth) / static_cast<_float>(GetInstance().m_iHeight);
}
