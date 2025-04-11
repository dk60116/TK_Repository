#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CScreen
{
	SINGLETONCLASS(CScreen);

public:
	void Start_Window(HWND _wnd);

public:
	HWND getHandle() { return m_hGameWnd; }
	const vector2Int& getResolution() { return m_v2Resolution; }

private:
	HWND m_hGameWnd;
	vector2Int m_v2Resolution;
	bool m_bFullScreen;
	_int m_iDPI;
};

END

