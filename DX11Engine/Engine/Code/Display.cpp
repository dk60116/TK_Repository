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

CRenderTarget* CDisplay::CreateRenderTarget(wstring _name, vector2Int _pos, vector2Int _size, ColorValue _color, const wstring& _psName)
{
	CRenderTarget* newTarget = CRenderTarget::Create(_name, _pos, _size, DXGI_FORMAT_R8G8B8A8_UNORM, _color, _psName);
	if (!newTarget)
		return nullptr;
	newTarget->AddRef();
	GetInstance().m_mRenderTargetList.emplace(newTarget->m_strTargetName, newTarget);

	return newTarget;
}

HRESULT CDisplay::CreateRenderTargets()
{
	if (!CreateRenderTarget(L"Diffuse", vector2Int::zero(), vector2Int(200, 200), ColorValue::magenta()))
		return E_FAIL;
	if (!CreateRenderTarget(L"Normal", vector2Int(200, 0), vector2(200, 200), ColorValue::black()))
		return E_FAIL;
	if (!CreateRenderTarget(L"Depth", vector2Int(400, 0), vector2(200, 200), ColorValue::black()))
		return E_FAIL;
	if (!CreateRenderTarget(L"Shading", vector2Int(0, 200), vector2Int(200, 200), ColorValue::white(), L"Shading"))
		return E_FAIL;
	if (!CreateRenderTarget(L"Specular", vector2Int(200, 200), vector2Int(200, 200), ColorValue::black()))
		return E_FAIL;
	if (!CreateRenderTarget(L"Combine", vector2Int(0, 0), CDisplay::Get_ScreenResolution(), ColorValue(0, 0, 0, 0), L"Combine"))
		return E_FAIL;

	return S_OK;
}

CRenderTarget* CDisplay::Get_RenderTarget(const wstring& _name)
{
	return GetInstance().m_mRenderTargetList[_name];
}

const _uint CDisplay::Get_RenderTargetCount()
{
	return static_cast<_uint>(GetInstance().m_mRenderTargetList.size());
}

void CDisplay::RenderTargetRender(const wstring& _name)
{
	GetInstance().m_mRenderTargetList[_name]->Render();
}

void CDisplay::ClearTargetRender(const wstring& _name)
{
	GetInstance().m_mRenderTargetList[_name]->Clear();
}
