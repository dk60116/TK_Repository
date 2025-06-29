#pragma once

#include "epch.h"

NS_BEGIN(Engine)

class ENGINE_DLL CEditor final
{
public:
	typedef struct EditopWindowOptionsDescription
	{
		UINT windowWidth = 1510;
		UINT windowHeight = 720;
		UINT hierachyWidth = 230;

	}EDITORWINOPTION;

	SINGLETONCLASS(CEditor);

public:
	HRESULT Initialize();
	void Release();

public:
	HWND Get_EditorWindow();
	void Editor_Update();

public:
	const EDITORWINOPTION& Get_Options();
	const vector2Int Get_ScreenResolution() const;

private:
	HWND CreateEditorWindow();
	static LRESULT CALLBACK EditorWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hEditorWindow;

private:
	class CHierachyBox* m_pHierachyBox;

private:
	EDITORWINOPTION m_sOptions;
};

NS_END

