#pragma once

#include "epch.h"

NS_BEGIN(Engine)

class ENGINE_DLL CEditor final
{
public:
	enum class TransformControleTool { VIEW, MOVE, ROTATE, SCALE, RECT, TRANSFORM };

	typedef struct EditopWindowOptionsDescription
	{
		UINT windowWidth = 1600;
		UINT windowHeight = 720;
		UINT topBarHeight = 28;
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
	void Editor_Update_During();
	void Editor_Update_End();

public:
	EDITORWINOPTION Get_Options() const;
	const vector2Int Get_WindowResolution() const;
	const vector2Int Get_ScreenResolution() const;

public:
	const TransformControleTool Get_ControleTool() const;
	void Change_ControleTool(const TransformControleTool _tool);
	const vector3 Get_EditorCamPositon() const;
	const quaternion Get_EditorCamQuaternion() const;
	void Set_EditorCamTransform(class CTransform* _transform);
	void Set_SelectedGameObject(class CGameObject* _target);
	void MoveTo_SelectedGameObject(class CGameObject* _target);
	CGameObject* Get_SelectedGameObject() const;

private:
	HWND m_hEditorWindow;

private:
	HWND CreateEditorWindow();
	static LRESULT CALLBACK EditorWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void ChangeControleTool();

	_bool m_bDoubleClicked;

	vector3 m_vCameraPos;
	quaternion m_vCameraQuat;
	_bool m_bIsMovingCamera;
	vector3 m_vCameraMoveStartPos;
	vector3 m_vCameraMoveTargetPos;
	_float m_fCameraMoveProgress;
	_float m_fCameraMoveDuration;

private:
	map<wstring, class CEditorBox*> m_mBoxList;

private:
	EDITORWINOPTION m_sOptions;

private:
	TransformControleTool m_eControleTool;
	CGameObject* m_pSelectedGameObject;
	CGameObject* m_pMoveTargetGameObject;
};

NS_END

