#pragma once

#include "Object.h"

NS_BEGIN(Engine)

class ENGINE_DLL CGraphicDevice final : public UObject
{
	SINGLETONCLASS(CGraphicDevice);

public:
	HRESULT Ready_GraphicDevice(HWND _hWnd, UINT _width, UINT _height);

public:
	ID3D11Device* Get_Device() const;
	ID3D11DeviceContext* Get_Context() const;

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;
};

NS_END
