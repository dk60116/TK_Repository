#pragma once

#include "epch.h"
#include "RenderTarget.h"

NS_BEGIN(Engine)

class ENGINE_DLL CDisplay final
{
	SINGLETONCLASS(CDisplay);

public:
	static HRESULT Initialize(HINSTANCE _hInst, HWND _hGameWnd, HWND _hEditorWnd);
	void Release();

public:
	static HINSTANCE Get_HInstance();
	static HWND Get_GameWindow();
	static HWND Get_EditorWindow();
	static vector2Int Get_ScreenResolution();
	static _float Get_Aspect();

public:
	static CRenderTarget* CreateRenderTarget(wstring _name, vector2Int _pos, vector2Int _size, ColorValue _color, const wstring& _psName = L"");
	static HRESULT CreateRenderTargets();

	static CRenderTarget* Get_RenderTarget(const wstring& _name);
	static const _uint Get_RenderTargetCount();

	static void RenderTargetRender(const wstring& _name);
	static void ClearTargetRender(const wstring& _name);

private:
	HINSTANCE m_hInst;
	HWND m_hGameWindow;
	HWND m_hEditorWindow;
	_bool m_bIsFullScreen;
	_uint m_iWidth, m_iHeight;

	map<wstring, CRenderTarget*> m_mRenderTargetList;
};

NS_END

