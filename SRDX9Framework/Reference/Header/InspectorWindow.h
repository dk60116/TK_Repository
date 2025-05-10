#pragma once

#include "EditorWindow.h"
#include "GameObject.h"

BEGIN(Engine)

struct InspectorViewOptions
{
	_int topHeight = 60;
	_int contstsBarHeight = 24;
};

struct InspectorPairView 
{
	HWND handle;
	void* value;
	void* prevValue;
	FieldType type = FieldType::FLOAT;
};

class ENGINE_DLL CInspectorWindow final
    : public CEditorWindow
{
	friend class CEngineEditor;

private:
    explicit CInspectorWindow();
    ~CInspectorWindow();

public:
	HRESULT Init(HWND _hWnd, vector2Int _size, _bool _isBase = false) override;
	void Render() override;
	void Update() override;
	void UpdateResolution(HWND _target, vector2Int _resolution) override;
	LRESULT CALLBACK WndProcHandle(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) override;
	void Destroy() override;

public:
	void ClearComponents();
	void ViewTargetInfor_GameObject(CGameObject* _target);
	void ViewTargetInfor_File();

private:
	void UpdateScrollInfo();

private:
	InspectorViewOptions m_sOptinos;
	HWND m_hEditName;
	vector<HWND> m_vContentsWindows, m_vChildWindows;
	CGameObject* m_pViewGameObject;
	_int m_iTotalHeight;
	_int m_iScrollPos;

	void CreateFloatBox(vector2Int _start, vector2Int _size, wstring _name, _float* _value);
	void CreateVector3Box(vector2Int _start, vector2Int _size, wstring _name, vector3* _value);
	void CreatePointerBox(vector2Int _start, vector2Int _size, wstring _name, UObject** _value);

	vector<InspectorPairView> m_vPairViewList;

private:
	HBRUSH m_hDarkBrush;
};

END;

