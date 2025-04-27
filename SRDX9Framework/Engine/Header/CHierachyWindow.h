#pragma once

#include "CEditorWindow.h"
#include "CEngineEditor.h"
#include "CGameObject.h"

BEGIN(Engine)

class ENGINE_DLL CHierachyWindow final
	: public CEditorWindow
{
	friend class CEngineEditor;

private:
	CHierachyWindow();
	~CHierachyWindow();

public:
	HRESULT Init(HWND _hWnd, vector2Int _size) override;
	void Render() override;
	void Update() override;
	void UpdateResolution(HWND _target, vector2Int _resolution) override;
	LRESULT CALLBACK WndProcHandle(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) override;

	HWND getTreeHandle() { return m_hTreeView; }
public:
	void BuildTree();
	void AddGameObjectRecursive(HTREEITEM _parentItem, CGameObject* _gameObject);

private:
	void SetTreeViewFont(int _fontSize);

private:
	HWND m_hTreeView;
	int m_iLeftSideWidth;
	HFONT m_hFont;
};

END

