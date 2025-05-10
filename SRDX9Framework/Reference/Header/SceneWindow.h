#pragma once

#include "EditorWindow.h"

BEGIN(Engine)

class ENGINE_DLL CSceneWindow 
    : public CEditorWindow
{
	friend class CEngineEditor;

private:
	explicit CSceneWindow();
	~CSceneWindow();

public:
	HRESULT Init(HWND _hWnd, vector2Int _size, _bool _isBase = false) override;
	void Render() override;
	void Update() override;
	void UpdateResolution(HWND _target, vector2Int _resolution) override;
};

END

