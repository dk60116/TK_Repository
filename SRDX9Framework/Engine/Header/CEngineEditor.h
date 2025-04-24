#pragma once

#include "UObject.h"
#include "Engine_Define.h"
#include "CEditorWindow.h"

BEGIN(Engine)

class ENGINE_DLL CEngineEditor
	: public UObject
{
	SINGLETONCLASS(CEngineEditor);

	void Release();

public:
	ATOM MyRegisterClass(HINSTANCE hInstance, WNDPROC _wndPrc);
	LRESULT CALLBACK WndProcHandle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	HINSTANCE getHInstance() { return m_hInst; }
	CEditorWindow* getWindow(wstring _name);

	const _bool isPlaying() const { return m_bPlaying; }
	void SetPlaying(const _bool _value) { m_bPlaying = _value; }
	void SwitchPlaying() { m_bPlaying = !m_bPlaying; }
	const _bool isPaused() const { return m_bPaused; }
	void SetPaused(const _bool _value) { m_bPaused = _value; };
	void SwitchPaused() { m_bPaused = !m_bPaused; }
	const _bool isNextFrame() const { return m_bNextFrame; }
	void SetNextFrame(const _bool _value) { m_bNextFrame = _value; }

	template <typename T>
	HRESULT CreateCustomWindow(HWND _window, const wstring _name, const vector2Int _size);

public:
	HFONT CreateDefaultFont(LPCWSTR _font, const _int _size, const _bool _bold = false);

private:
	HINSTANCE m_hInst;
	map<wstring, CEditorWindow*> m_mWindowList;

	_bool m_bPlaying;
	_bool m_bPaused;
	_bool m_bNextFrame;
};

END

template<typename T>
inline HRESULT CEngineEditor::CreateCustomWindow(HWND _window, const wstring _name, const vector2Int _size)
{
	if (_window == nullptr)
		return E_FAIL;

	T* newWindow = new T();

	if (newWindow == nullptr)
		return E_FAIL;

	newWindow->AddRef();

	m_mWindowList.insert({ _name, newWindow });
	
	if (FAILED(newWindow->Init(_window, _size)))
		return E_FAIL;

	return S_OK;
}
