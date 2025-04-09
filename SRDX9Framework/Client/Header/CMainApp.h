#pragma once

#include "Base.h"
#include "Engine_Define.h"

#include "CGraphicDev.h"

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

class CMainApp : public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT Ready_MainApp();
	_int Update_MainApp();
	void LateUpdate_MainApp();
	void Render_MainApp();

private:
	CGraphicDev* m_pDevClass;
	LPDIRECT3DDEVICE9	m_pGraphicDev;

public:
	static CMainApp* Create();

private:
	virtual void Free() override;

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
};

