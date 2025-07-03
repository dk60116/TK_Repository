#pragma once

#include "epch.h"

NS_BEGIN(Engine)

class ENGINE_DLL CEditor final
{
public:
	typedef struct EditopWindowOptionsDescription
	{
		UINT windowWidth = 1600;
		UINT windowHeight = 720;
		UINT hierachyWidth = 250;
		UINT inspectorWidth = 260;

	}EDITORWINOPTION;

	SINGLETONCLASS(CEditor);

public:
	HRESULT Initialize();
	void Release();

public:
	HWND Get_EditorWindow();
	void Editor_Update_Begin();
	void Editor_Update_End();

public:
	EDITORWINOPTION Get_Options() const;
	const vector2Int Get_ScreenResolution() const;

public:
	void Set_SelectedGameObject(class CGameObject* _target);
	CGameObject* Get_SelectedGameObject() const;

private:
	HWND CreateEditorWindow();
	static LRESULT CALLBACK EditorWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hEditorWindow;

private:
	class CEditorBox* m_pHierachyBox;
	class CEditorBox* m_pInspectorBox;

	map<wstring, CEditorBox*> m_mBoxList;

private:
	EDITORWINOPTION m_sOptions;

private:
	CGameObject* m_pSelectedGameObject;
};

NS_END

