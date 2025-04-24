#pragma once

#include "CEditorWindow.h"
#include "CEngineEditor.h"

BEGIN(Engine)

class ENGINE_DLL CBaseWindow final
	: public CEditorWindow
{
	friend class CEngineEditor;

private:
	CBaseWindow();
	~CBaseWindow();

public:
	HRESULT Init(HWND _hWnd, vector2Int _size) override;
	void Render() override;
	void Update() override;
	void UpdateResolution(HWND _target, vector2Int _resolution) override;
	LRESULT CALLBACK WndProcHandle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

private:
	HWND m_hBottomBar;
	HWND m_hPlayBtn, m_hPauseBtn, m_hOneFrameBtn;
};

END

