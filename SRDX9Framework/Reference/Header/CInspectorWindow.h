#pragma once

#include "CEditorWindow.h"
#include "CGameObject.h"

BEGIN(Engine)

struct InspectorViewOptions
{
	_int topHeight = 60;
	_int contstsBarHeight = 24;
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
	vector<HWND> m_vContentsWindows;
	CGameObject* m_pViewGameObject;
	_int m_iTotalHeight;
	_int m_iScrollPos;

private:
	HBRUSH m_hDarkBrush;
};

END;

