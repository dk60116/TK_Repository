#pragma once

#include "Engine_Define.h"
#include "UObject.h"

BEGIN(Engine)

class IDeviceResetListener
	: UObject
{
public:
	virtual void OnDeviceLost() PURE;
	virtual void OnDeviceReset(LPDIRECT3DDEVICE9 pDevice, _uint _width, _uint _height) PURE;
	virtual ~IDeviceResetListener() = default;

public:
	void RegisterResetListener(IDeviceResetListener* pListener);

private:
	vector<IDeviceResetListener*> m_vecResetListeners;
};

class ENGINE_DLL CGraphicDev
	: public UObject
{
	SINGLETONCLASS(CGraphicDev);

public:
	LPDIRECT3DDEVICE9 Get_GraphicDev();

public:
	HRESULT Ready_GraphicDev(HWND _hWnd, 
								WINMODE _eMode, 
								const _uint& _iSizeX, 
								const _uint& _iSizeY, 
								CGraphicDev * *_ppGraphicDev);

	void Render_Begin(D3DVIEWPORT9 _viewPort, D3DXCOLOR _color);
	void Render_End(HWND _window);

	HRESULT ReSize(_uint _newWidth, _uint _newHeight);

private:
	LPDIRECT3D9 m_pSDK;
	LPDIRECT3DDEVICE9 m_pGraphicDev;
public:
	virtual void Free();
};

END

