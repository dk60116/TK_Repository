#pragma once

#include "CEditorWindow.h"

BEGIN(Engine)

class ENGINE_DLL CGameWIndow :
    public CEditorWindow
{
	friend class CEngineEditor;

private:
	CGameWIndow();
	~CGameWIndow();

public:
	HRESULT Init(HWND _hWnd, vector2Int _size) override;
	void Render() override;
	void Update() override;
	void UpdateResolution(HWND _target, vector2Int _resolution) override;
};

END

