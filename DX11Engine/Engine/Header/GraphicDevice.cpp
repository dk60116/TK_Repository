#include "epch.h"
#include "GraphicDevice.h"

CGraphicDevice::CGraphicDevice()
	: m_pDevice(nullptr)
	, m_pContext(nullptr)
{
}

CGraphicDevice::~CGraphicDevice()
{
}

HRESULT CGraphicDevice::Ready_GraphicDevice(HWND _hWnd, UINT _width, UINT _height)
{
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Width = _width;
	swapDesc.BufferDesc.Height = _height;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = _hWnd;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.Windowed = TRUE;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	IDXGISwapChain* pSwapChain = nullptr;

	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 0, nullptr, 0, D3D11_SDK_LAYERS_VERSION,
		&swapDesc, &pSwapChain,
		&m_pDevice, nullptr, &m_pContext
	);

	if (FAILED(hr))
		return E_FAIL;

	if (pSwapChain)
		pSwapChain->Release();

	return S_OK;
}

ID3D11Device* CGraphicDevice::Get_Device() const
{
	return m_pDevice;
}

ID3D11DeviceContext* CGraphicDevice::Get_Context() const
{
	return m_pContext;
}
