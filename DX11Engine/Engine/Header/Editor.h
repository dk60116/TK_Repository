#pragma once

#include "epch.h"

NS_BEGIN(Engine)

class ENGINE_DLL CEditor final
{
public:
	enum class TransformControleTool { VIEW, MOVE, ROTATE, SCALE, RECT, TRANSFORM };

	typedef struct EditopWindowOptionsDescription
	{
		_uint windowWidth = 1750;
		_uint windowHeight = 720;
		_uint topBarHeight = 28;
		_uint projectWidth = 220;
		_uint hierachyWidth = 250;
		_uint inspectorWidth = 260;

	}EDITORWINOPTION;

	SINGLETONCLASS(CEditor);

public:
	static HRESULT Initialize();
	static void Release();

public:
	static void Play();
	static void Stop();
	static void Pause();

	static const _bool IsPlaying();

public:
	static HWND Get_EditorWindow();
	static void Editor_Update_Begin();
	static void Editor_Update_During();
	static void Editor_Update_End();

public:
	static EDITORWINOPTION Get_Options();
	static vector2Int Get_WindowResolution();
	static vector2Int Get_ScreenResolution();

public:
	static TransformControleTool Get_ControleTool();
	static void Change_ControleTool(const TransformControleTool _tool);
	static vector3 Get_EditorCamPositon();
	static quaternion Get_EditorCamQuaternion();
	static void Set_EditorCamTransform(class CTransform* _transform);
	static void Set_SelectedGameObject(class CGameObject* _target);
	static void MoveTo_SelectedGameObject(class CGameObject* _target);
	static CGameObject* Get_SelectedGameObject();

private:
	static HWND CreateEditorWindow();
	static LRESULT CALLBACK EditorWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static void ChangeControleTool();

private:
	HWND m_hEditorWindow;

	_bool m_bDoubleClicked;

	vector3 m_vCameraPos;
	quaternion m_vCameraQuat;
	_bool m_bIsMovingCamera;
	vector3 m_vCameraMoveStartPos;
	vector3 m_vCameraMoveTargetPos;
	_float m_fCameraMoveProgress;
	_float m_fCameraMoveDuration;

private:
	_bool m_bPlaying, m_bPrevPlaying;
	map<wstring, class CEditorBox*> m_mBoxList;

private:
	EDITORWINOPTION m_sOptions;

private:
	TransformControleTool m_eControleTool;
	CGameObject* m_pSelectedGameObject;
	CGameObject* m_pMoveTargetGameObject;

private:
	HICON m_hEditorWindowIcon_Default, m_hEditorWindoIcon_Small;
};

NS_END

