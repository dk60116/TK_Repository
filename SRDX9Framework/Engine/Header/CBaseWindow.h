#pragma once

#include "CEditorWindow.h"

BEGIN(Engine)

class ENGINE_DLL CBaseWindow final
	: public CEditorWindow
{
	friend class EngineEditor;

private:
	CBaseWindow(HWND _hWnd);
	~CBaseWindow();

public:
	void Init(HWND _hWnd, vector2Int _size) override;
	void Render() override;
	void Update() override;
	void UpdateResolution(vector2Int _resolutoin) override;

private:
	HWND m_hPlayBtn, m_hPauseBtn, m_hOneFrameBtn;
};

END

