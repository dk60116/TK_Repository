#pragma once

#include "Object.h"

NS_BEGIN(Engine)

class ENGINE_DLL CGraphicDevice final : public UObject
{
	SINGLETONCLASS(CGraphicDevice);

public:
	HRESULT Initialize();
	void Destroy();

public:
	HRESULT Ready_GraphicDevice(HWND _hWnd, vector2Int _resolution);

	HRESULT Clear_BackBuffer_View(const ColorValue* _clearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();

	HRESULT ReSize(_uint _newWidth, _uint _newHeight);

public:
	ID3D11Device* Get_Device() const;
	ID3D11DeviceContext* Get_Context() const;

private:
	HRESULT Ready_SwapChain(HWND _hWnd, WINMODE _isWindowed, _uint _winWidth, _uint _winHeight);
	HRESULT Ready_BackBufferRenderTargetView();
	HRESULT Ready_DepthStencilView(_uint _winWidth, _uint _winHeight);

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;

	IDXGISwapChain* m_pSwapChain = { nullptr };

	ID3D11RenderTargetView* m_pBackBufferRTV;
	ID3D11DepthStencilView* m_pDepthStencilView;
};

NS_END
