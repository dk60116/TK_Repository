#pragma once

#include "Engine_Define.h"
#include "CEditorWindow.h"
#include "CEngineEditor.h"

BEGIN(Engine)

class ENGINE_DLL CHierachyWindow final
	: public CEditorWindow
{
	friend class EngineEditor;

private:
	CHierachyWindow(HWND _hWnd);
	~CHierachyWindow();

public:
	void Init(HWND _hWnd, vector2Int _size) override;
	void Render() override;
	void Update() override;
	void UpdateResolution(vector2Int _resolutoin) override;

private:
	HWND m_hTreeView;
};

END

