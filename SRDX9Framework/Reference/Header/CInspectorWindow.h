#pragma once

#include "CEditorWindow.h"

BEGIN(Engine)

class ENGINE_DLL CInspectorWindow final
    : public CEditorWindow
{
private:
    explicit CInspectorWindow();
    ~CInspectorWindow();

public:
	HRESULT Init(HWND _hWnd, vector2Int _size, _bool _isBase = false) override;
	void Render() override;
	void Update() override;
	void UpdateResolution(HWND _target, vector2Int _resolution) override;
};

END;

