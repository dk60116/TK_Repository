#pragma once

#include "UObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CEditorWindow abstract
	: public UObject
{
protected:
	CEditorWindow();
	~CEditorWindow();

public:
	virtual void Init(HWND _hWnd, vector2Int _size);
	virtual void Render() PURE;
	virtual void Update() PURE;
	virtual void UpdateResolution(vector2Int _resolutoin);

protected:
	HWND m_hWnd;
	HWND m_hTopBar;

	vector2Int m_v2Resolution;
};

END

