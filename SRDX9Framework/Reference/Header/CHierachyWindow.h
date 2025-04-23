#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CHierachyWindow
{
	SINGLETONCLASS(CHierachyWindow);

public:
	void Init(HWND _hWnd, vector2Int _size);
	void Render();
	void Update();
	void HandleMessage(UINT _msg, WPARAM _wParam, LPARAM _lParam);

private:
	HWND m_hWnd;
	HWND m_hTreeView;
};

END

