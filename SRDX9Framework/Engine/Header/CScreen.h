#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CScreen
{
	SINGLETONCLASS(CScreen);

public:
	void Start_Window(HINSTANCE _hInst, HWND _hMainWnd, HWND _hSceneWnd, HWND _hGameWnd);

public:
	HINSTANCE getHInstance() { return m_hInstance; }
	HWND getMainHandle() { return m_hMainWnd; }
	HWND getSceneHandle() { return m_hSceneWnd; }
	HWND getGameHandle() { return m_hGameWnd; }
	const vector2Int& getResolution() { return m_v2Resolution; }
	const _int& getDPI() { return m_iDPI; }

public:
	void UpdateResolution(const _int& _width, const _int& _height);

private:
	HINSTANCE m_hInstance;
	HWND m_hMainWnd, m_hSceneWnd, m_hGameWnd;
	vector2Int m_v2Resolution;
	bool m_bFullScreen;
	_int m_iDPI;
};

END

