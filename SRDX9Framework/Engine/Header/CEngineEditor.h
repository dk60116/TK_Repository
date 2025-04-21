#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CEngineEditor
{
	SINGLETONCLASS(CEngineEditor);

public:
	void Init_Main(HINSTANCE _hInst, HWND _mainWnd);
	void Init_Scene(HWND _sceneWnd);
	void Init_Game(HWND _gameWnd);

public:
	LRESULT CALLBACK WndProcHandle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	HINSTANCE getHInst() { return m_hInst; }
	HWND getMainHandle() { return m_hMainWnd; }

	const _bool& isPlaying() const { return m_bPlaying; }
	const _bool& isPaused() const { return m_bPaused; }
	void SetPaused(const _bool _value) { m_bPaused = _value; };
	const _bool& isNextFrame() const { return m_bNextFrame; }
	void SetNextFrame(const _bool _value) { m_bNextFrame = _value; }

	HWND getMainTopBar() { return m_hTopBar; }
	HWND getMainBottomBar() { return m_hBottomBar; }
	HWND getSceneTopBar() { return m_hTop_Scene; }
	HWND getGameTopBar() { return m_hTop_Game; }

	void UpdateResolution(vector2Int _resolution);

private:
	HFONT CreateDefaultFont(LPCWSTR _font, _float _size, _bool _bold = false);

private:
	HINSTANCE m_hInst;
	HWND m_hMainWnd;

	HWND m_hTopBar, m_hTop_Scene, m_hTop_Game;
	HWND m_hBottomBar;
	HWND m_hBtnPause;
	HWND m_hBtnPlay;
	HWND m_hBtnNextFrame;

	_bool m_bPlaying;
	_bool m_bPaused;
	_bool m_bNextFrame;
};

END

