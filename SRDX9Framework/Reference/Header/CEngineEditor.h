#pragma once

#include "Object.h"
#include "Engine_Define.h"
#include "CTime.h"
#include "CEditorWindow.h"
#include "CBaseWindow.h"
#include "CSceneWindow.h"
#include "CGameWindow.h"
#include "CHierachyWindow.h"
#include "CInspectorWindow.h"
#include "GameObject.h"

BEGIN(Engine)

struct Engine_WindowOptions
{
	ColorValue baseColor = ColorValue(56, 56, 56);
	ColorValue baseTextColor = ColorValue(167, 167, 167);
	ColorValue textBoxColor = ColorValue(42, 42, 42);
};

class ENGINE_DLL CEngineEditor
	: public UObject
{
	SINGLETONCLASS(CEngineEditor);

	void Release();

public:
	ATOM MyRegisterClass(HINSTANCE hInstance, WNDPROC _wndPrc);
	LRESULT CALLBACK WndProcHandle(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

public:
	void Update();
	void LateUpdate();

public:
	Engine_WindowOptions& getOptions() { return m_sEngineOptions; }
	HINSTANCE getHInstance() { return m_hInst; }
	template <typename T>
	T* getWindow();
	HWND FindWindowHandle(const wstring _window);

	const _bool isPlaying() const { return m_bPlaying; }
	void SetPlaying(const _bool _value) { m_bPlaying = _value; }
	void SwitchPlaying() { m_bPlaying = !m_bPlaying; }
	const _bool isPaused() const { return m_bPaused; }
	void SetPaused(const _bool _value) { m_bPaused = _value; };
	void SwitchPaused() { m_bPaused = !m_bPaused; }
	const _bool isNextFrame() const { return m_bNextFrame; }
	void SetNextFrame(const _bool _value) { m_bNextFrame = _value; }

	template <typename T>
	HRESULT CreateCustomWindow(HWND _hWnd, const wstring _name, const vector2Int _size);

	void SelectGameObject(CGameObject* _gameObject);

	CGameObject* getSelectedGameObject() const { return m_pSelectedGameObject; }

public:
	static void RemoveBtnsAndRoundedCorners(HWND _hWnd);
	static void BeautifyEdit(HWND _hEdit);
	static LRESULT CALLBACK EditSubclassProc(HWND hWnd, UINT msg,
		WPARAM wParam, LPARAM lParam,
		UINT_PTR, DWORD_PTR);
	static HFONT CreateDefaultFont(LPCWSTR _font, const _int _size, const _bool _bold = false);

private:
	void RegisterBaseColorPanelClass();

private:
	Engine_WindowOptions m_sEngineOptions;
	HINSTANCE m_hInst;
	map<wstring, CEditorWindow*> m_mWindowList;
	map<wstring, HWND> m_mWHandleList;

	_bool m_bPlaying;
	_bool m_bPaused;
	_bool m_bNextFrame;

	CGameObject* m_pSelectedGameObject;
};

END

template<typename T>
inline T* CEngineEditor::getWindow()
{
	for (TRAVERSAL_ITER(m_mWindowList, it))
	{
		T* result = dynamic_cast<T*>((*it).second);
		if (result)
			return result;
	}

	return nullptr;
}

template<typename T>
inline HRESULT CEngineEditor::CreateCustomWindow(HWND _hWnd, const wstring _name, const vector2Int _size)
{
	if (_hWnd == nullptr)
		return E_FAIL;

	T* newWindow = new T();

	if (newWindow == nullptr)
		return E_FAIL;

	newWindow->AddRef();

	m_mWindowList.insert({ _name, newWindow });
	m_mWHandleList.insert({ _name, _hWnd });
	
	if (FAILED(newWindow->Init(_hWnd, _size)))
		return E_FAIL;

	return S_OK;
}
