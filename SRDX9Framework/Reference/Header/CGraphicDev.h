#pragma once

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class IDeviceResetListener
{
public:
	virtual void OnDeviceLost() = 0;
	virtual void OnDeviceReset(LPDIRECT3DDEVICE9 pDevice, _uint _width, _uint _height) PURE;
	virtual ~IDeviceResetListener() = default;

public:
	void RegisterResetListener(IDeviceResetListener* pListener);

private:
	vector<IDeviceResetListener*> m_vecResetListeners;
};

class ENGINE_DLL CGraphicDev : public CBase
{
	SINGLETONCLASS(CGraphicDev);

public:
	LPDIRECT3DDEVICE9	Get_GraphicDev() { return m_pGraphicDev; }

public:
	HRESULT Ready_GraphicDev(HWND hWnd, 
								WINMODE eMode, 
								const _uint& iSizeX, 
								const _uint& iSizeY, 
								Engine::CGraphicDev * *ppGraphicDev);

	void	Render_Begin(D3DXCOLOR _color);
	void	Render_End();

	HRESULT ReSize(_uint _newWidth, _uint _newHeight);

private:
	LPDIRECT3D9				m_pSDK;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
public:
	virtual void Free();
};

END

