#pragma once

#include "CEditorWindow.h"
#include "CEngineEditor.h"
#include "GameObject.h"

BEGIN(Engine)

struct HierachyViewOptions
{
	_int fontSize = 14;
	ColorValue textColor = ColorValue::white();
	ColorValue leftSideColor;
};

class ENGINE_DLL CHierachyWindow final
	: public CEditorWindow
{
	friend class CEngineEditor;

private:
	explicit CHierachyWindow();
	~CHierachyWindow();

public:
	HRESULT Init(HWND _hWnd, vector2Int _size, _bool _isBase = false) override;
	void Render() override;
	void Update() override;
	void UpdateResolution(HWND _target, vector2Int _resolution) override;
	LRESULT CALLBACK WndProcHandle(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) override;

	HWND getTreeHandle() { return m_hTreeView; }
public:
	void BuildTree();
	void AddSceneRecursive(CScene* _scene);
	void AddGameObjectRecursive(HTREEITEM _parentItem, CGameObject* _gameObject);

private:
	void SetTreeViewOptions();

public:
	const _bool IsTreeViewHighlightEmpty();
	static LRESULT CALLBACK TreeSubProc
	(
		HWND _hWnd, UINT _msg,
		WPARAM _wParam, LPARAM _lParam,
		UINT_PTR _idSubClass, DWORD_PTR _dwRefData
	);

public:
	void AdjustTreeHeight();

private:
	bool GetLastVisibleItemRect(RECT& _rcOut);

private:
	HierachyViewOptions m_sOptions;
	HWND m_hTreeView, m_hLeftSideArea;
	HTREEITEM m_hPressedItem;
	_int m_iLeftSideWidth;
	HFONT m_hFont;
};

END

