#pragma once

#include "CEditorWindow.h"
#include "EngineEditor.h"

BEGIN(Engine)

class ENGINE_DLL CBaseWindow final
	: public CEditorWindow
{
	friend class CEngineEditor;

private:
	explicit CBaseWindow();
	~CBaseWindow();

public:
	HRESULT Init(HWND _hWnd, vector2Int _size, _bool _isBase  = false) override;
	void Render() override;
	void Update() override;
	void UpdateResolution(HWND _target, vector2Int _resolution) override;
	LRESULT CALLBACK WndProcHandle(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) override;

private:
	HWND m_hBottomBar;
	HWND m_hPlayBtn, m_hPauseBtn, m_hOneFrameBtn;
};

END

