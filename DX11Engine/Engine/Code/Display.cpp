#include "epch.h"
#include "Display.h"

CDisplay::CDisplay()
	: m_hInst(nullptr)
	, m_hGameWindow(nullptr)
	, m_hEditorWindow(nullptr)
	, m_bIsFullScreen(false)
	, m_iWidth(1280)
	, m_iHeight(720)
	, m_mRenderTargetList({})
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
	for (TRAVERSAL_ITER(m_mRenderTargetList, it))
		Safe_Release((*it).second);
	
	m_mRenderTargetList.clear();
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

CRenderTarget* CDisplay::CreateRenderTarget(wstring _name, vector2Int _pos, vector2Int _size, ColorValue _color)
{
	CRenderTarget* newTarget = CRenderTarget::Create(_name, _pos, _size, DXGI_FORMAT_R8G8B8A8_UNORM, _color);
	newTarget->AddRef();
	GetInstance().m_mRenderTargetList.emplace(newTarget->m_strTargetName, newTarget);

	return newTarget;
}

HRESULT CDisplay::CreateRenderTargets()
{
	if (!CreateRenderTarget(L"Diffuse", vector2Int::zero(), vector2Int(200.f, 200.f), ColorValue::white()))
		return E_FAIL;
	if (!CreateRenderTarget(L"Normal", vector2Int(200.f, 0.f), vector2(200.f, 200.f), ColorValue::white()))
		return E_FAIL;
	if (!CreateRenderTarget(L"Shading", vector2Int(0.f, 200.f), vector2Int(200.f, 200.f), ColorValue::white()))
		return E_FAIL;
	if (!CreateRenderTarget(L"Specular", vector2Int(200.f, 200.f), vector2Int(200.f, 200.f), ColorValue::black()))
		return E_FAIL;

	return S_OK;
}

void CDisplay::RenderTargetRender(const wstring& _name)
{
	GetInstance().m_mRenderTargetList[_name]->Render();
}

void CDisplay::ClearTargetRender(const wstring& _name)
{
	GetInstance().m_mRenderTargetList[_name]->Clear();
}
