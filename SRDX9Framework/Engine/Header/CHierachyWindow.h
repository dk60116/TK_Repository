#pragma once

#include "CEditorWindow.h"
#include "CEngineEditor.h"

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

private:
	HWND m_hTreeView;
};

END

