#pragma once

#include "Engine_Define.h"
#include "CEditorWindow.h"

BEGIN(Engine)

class ENGINE_DLL CEngineEditor
{
	SINGLETONCLASS(CEngineEditor);

public:
	void Init_Main(HINSTANCE _hInst, HWND _mainWnd);
	void Init_Scene(HWND _sceneWnd);
	void Init_Game(HWND _gameWnd);

public:
	ATOM MyRegisterClass(HINSTANCE hInstance, WNDPROC _wndPrc);
	LRESULT CALLBACK WndProcHandle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	HINSTANCE getHInst() { return m_hInst; }
	HWND getMainHandle() { return m_hMainWnd; }

	const _bool isPlaying() const { return m_bPlaying; }
	const _bool isPaused() const { return m_bPaused; }
	void SetPaused(const _bool _value) { m_bPaused = _value; };
	const _bool isNextFrame() const { return m_bNextFrame; }
	void SetNextFrame(const _bool _value) { m_bNextFrame = _value; }

	HWND getMainTopBar() { return m_hTopBar; }
	HWND getMainBottomBar() { return m_hBottomBar; }

	void UpdateBaseResolution(const vector2Int _resolution);
	void UpdateSceneResolution(const vector2Int _resolution);

	template <typename T>
	HRESULT* CreateCustomWindow(HWND _window, wstring _name, const vector2Int _size);

	CEditorWindow* getWindow(wstring _name);

private:
	HFONT CreateDefaultFont(LPCWSTR _font, const _int _size, const _bool _bold = false);

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

	map<wstring, CEditorWindow*> m_mWindowList;
};

END

template<typename T>
inline HRESULT* CEngineEditor::CreateCustomWindow(HWND _window, wstring _name, const vector2Int _size)
{
	T* newWindow = new T();

	if (newWindow == nullptr)
		return E_FAIL;

	m_mWindowList.insert({ _name, newWindow });
	newWindow->Init(_window, _size);

	return S_OK;
}
