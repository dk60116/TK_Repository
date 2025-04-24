#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CScreen
{
	SINGLETONCLASS(CScreen);

public:
	void Start_Window(HINSTANCE _hInst, int _cmdShow);

public:
	HINSTANCE getHInstance() { return m_hInstance; }

	HWND getWindowHandle(wstring _window);

	const vector2Int getSceneResolution() { return m_v2SceneResolution; }
	const vector2Int getGameResolution() { return m_v2GameResolution; }

public:
	void UpdateSceneResolution(const UINT _width, const UINT _height);
	void UpdateGameResolution(const UINT _width, const UINT _height);

	void RemoveBtnsAndRoundedCorners(HWND _hWnd);

private:
	HINSTANCE m_hInstance;
	map<wstring, HWND> m_mWHandleList;
	vector2Int m_v2SceneResolution, m_v2GameResolution;
	bool m_bFullScreen;
};

END

