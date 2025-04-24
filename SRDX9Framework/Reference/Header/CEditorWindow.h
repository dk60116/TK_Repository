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
	virtual HRESULT Init(HWND _hWnd, const vector2Int _size);
	virtual void Render() PURE;
	virtual void Update() PURE;
	virtual void UpdateResolution(HWND _target, const vector2Int _resolution);
	virtual void Destroy();
	virtual LRESULT CALLBACK WndProcHandle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
	HWND m_hWnd;
	HWND m_hTopBar;
	UINT m_iTopBarHeight;

	vector2Int m_v2Resolution;

	RECT m_sClinentRect, m_sWindowRect;
};

END

