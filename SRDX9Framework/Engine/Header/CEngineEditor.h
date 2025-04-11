#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CEngineEditor
{
	SINGLETONCLASS(CEngineEditor);

public:
	void Init(HINSTANCE _hInst, HWND _mainWnd);

public:
	HINSTANCE getHInst() { return m_hInst; }
	HWND getMainHandle() { return m_hMainWnd; }

private:
	HINSTANCE m_hInst;
	HWND m_hMainWnd;
};

END

