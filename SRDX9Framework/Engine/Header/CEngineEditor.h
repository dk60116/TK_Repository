#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CEngineEditor
{
	SINGLETONCLASS(CEngineEditor);

public:
	enum GameRunningState { RUNNING, PAUSED, STOPPED };

public:
	void Init_Main(HINSTANCE _hInst, HWND _mainWnd);
	void Init_Scene(HWND _sceneWnd);
	void Init_Game(HWND _gameWnd);

public:
	LRESULT CALLBACK WndProcHandle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	HINSTANCE getHInst() { return m_hInst; }
	HWND getMainHandle() { return m_hMainWnd; }

	GameRunningState HandleCommand(WPARAM wParam);

	void SetPaused(const bool _value) { m_bPaused = _value; };
	const bool& IsPaused() { return m_bPaused; }

	HWND getMainTopBar() { return m_hTopBar; }
	HWND getSceneTopBar() { return m_hTop_Scene; }
	HWND getGameTopBar() { return m_hTop_Game; }

	void UpdateResolution(vector2Int _resolution);

private:
	HINSTANCE m_hInst;
	HWND m_hMainWnd;

	HWND m_hTopBar, m_hTop_Scene, m_hTop_Game;
	HWND m_hBtnPause;
	HWND m_hBtnStop;
	HWND m_hBtnNextFrame;

	_bool m_bPaused;
};

END

