#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CEngineEditor
{
	SINGLETONCLASS(CEngineEditor);

public:
	enum GameRunningState { RUNNING, PAUSED, STOPPED };

public:
	void Init(HINSTANCE _hInst, HWND _mainWnd);

public:
	HINSTANCE getHInst() { return m_hInst; }
	HWND getMainHandle() { return m_hMainWnd; }

	GameRunningState HandleCommand(WPARAM wParam);

	void SetPaused(const bool _value) { m_bPaused = _value; };
	const bool& IsPaused() { return m_bPaused; }

	HWND getTopBar() { return m_hTopBar;  }

	LRESULT CALLBACK UpdateProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void UpdateResolution(vector2Int _resolution);

private:
	HINSTANCE m_hInst;
	HWND m_hMainWnd;

	HWND m_hTopBar;
	HWND m_hBtnPause;
	HWND m_hBtnStop;
	HWND m_hBtnNextFrame;

	_bool m_bPaused;
};

END

