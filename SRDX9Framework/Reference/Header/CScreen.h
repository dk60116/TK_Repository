#pragma once

#include "UObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CScreen
	: public UObject
{
	SINGLETONCLASS(CScreen);

public:
	HRESULT Start_Window(HINSTANCE _hInst, int _cmdShow);

	const vector2Int getSceneResolution() { return m_v2SceneResolution; }
	const vector2Int getGameResolution() { return m_v2GameResolution; }

public:
	void UpdateSceneResolution(const UINT _width, const UINT _height);
	void UpdateGameResolution(const UINT _width, const UINT _height);

private:
	HINSTANCE m_hInstance;
	vector2Int m_v2SceneResolution, m_v2GameResolution;
	bool m_bFullScreen;
};

END

