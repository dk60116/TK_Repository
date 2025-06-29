#pragma once

#include "epch.h"

NS_BEGIN(Engine)

class ENGINE_DLL CEditor final
{
	SINGLETONCLASS(CEditor);

public:
	HRESULT Initialize();
	void Release();

public:
	HWND Get_EditorWindow();

private:
	HWND CreateEditorWindow();
	static LRESULT CALLBACK EditorWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hEditorWindow;
};

NS_END

