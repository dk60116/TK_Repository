#pragma once

#include "epch.h"

NS_BEGIN(Engine)

class ENGINE_DLL CDisplay final
{
	SINGLETONCLASS(CDisplay);

public:
	static HRESULT Initialize(HINSTANCE _hInst, HWND _hGameWnd, HWND _hEditorWnd);

public:
	static HINSTANCE Get_HInstance();
	static HWND Get_GameWindow();
	static HWND Get_EditorWindow();
	static vector2Int Get_ScreenResolution();
	static _float Get_Aspect();

private:
	HINSTANCE m_hInst;
	HWND m_hGameWindow;
	HWND m_hEditorWindow;
	_bool m_bIsFullScreen;
	_uint m_iWidth, m_iHeight;
};

NS_END

