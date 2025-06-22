#include "epch.h"
#include "GraphicDevice.h"

CGraphicDevice::CGraphicDevice()
	: m_pDevice(nullptr)
	, m_pContext(nullptr)
	, m_pSwapChain(nullptr)
	, m_pBackBufferRTV(nullptr)
	, m_pDepthStencilView(nullptr)
{
}

CGraphicDevice::~CGraphicDevice()
{
	Destroy();
}

HRESULT CGraphicDevice::Initialize()
{
	if (FAILED(Ready_GraphicDevice(CDisplay::GetInstance().Get_GameWindow(), CDisplay::GetInstance().Get_ScreenResolution())))
		return E_FAIL;

	vector2Int resolution = CDisplay::GetInstance().Get_ScreenResolution();

	if (FAILED(Ready_BackBufferRenderTargetView()))
		return E_FAIL;

	if (FAILED(Ready_DepthStencilView(resolution.x, resolution.y)))
		return E_FAIL;

	m_pContext->OMSetRenderTargets(1, &m_pBackBufferRTV, m_pDepthStencilView);

	return S_OK;
}

void CGraphicDevice::Destroy()
{
	Safe_Release(m_pSwapChain);
	Safe_Release(m_pDepthStencilView);
	Safe_Release(m_pBackBufferRTV);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}

HRESULT CGraphicDevice::Ready_GraphicDevice(HWND _hWnd, vector2Int _resolution)
{
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Width = _resolution.x;
	swapDesc.BufferDesc.Height = _resolution.y;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = _hWnd;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.Windowed = TRUE;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	IDXGISwapChain* pSwapChain = nullptr;

	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 0, nullptr, 0,
		D3D11_SDK_VERSION,
		&swapDesc, &m_pSwapChain,
		&m_pDevice, nullptr, &m_pContext
	);

	if (FAILED(hr))
		return E_FAIL;

	if (pSwapChain)
		pSwapChain->Release();

	return S_OK;
}

HRESULT CGraphicDevice::Clear_BackBuffer_View(const ColorValue* _clearColor)
{
	if (!m_pContext)
		return E_FAIL;

	m_pContext->ClearRenderTargetView(m_pBackBufferRTV, reinterpret_cast<const _float*>(_clearColor));

	return S_OK;
}

HRESULT CGraphicDevice::Clear_DepthStencil_View()
{
	if (!m_pContext)
		return E_FAIL;

	m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	return S_OK;
}

HRESULT CGraphicDevice::Present()
{
	if (nullptr == m_pSwapChain)
		return E_FAIL;

	return m_pSwapChain->Present(0, 0);
}

ID3D11Device* CGraphicDevice::Get_Device() const
{
	return m_pDevice;
}

ID3D11DeviceContext* CGraphicDevice::Get_Context() const
{
	return m_pContext;
}

HRESULT CGraphicDevice::Ready_SwapChain(HWND _hWnd, WINMODE _isWindowed, _uint _winWidth, _uint _winHeight)
{
	IDXGIDevice* pDevice = nullptr;
	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDevice);

	IDXGIAdapter* pAdapter = nullptr;
	pDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter);

	IDXGIFactory* pFactory = nullptr;
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

	DXGI_SWAP_CHAIN_DESC		SwapChain;
	ZeroMemory(&SwapChain, sizeof(DXGI_SWAP_CHAIN_DESC));

	SwapChain.BufferDesc.Width = _winWidth;
	SwapChain.BufferDesc.Height = _winHeight;

	SwapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	SwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChain.BufferCount = 1;

	SwapChain.BufferDesc.RefreshRate.Numerator = 60;
	SwapChain.BufferDesc.RefreshRate.Denominator = 1;

	SwapChain.SampleDesc.Quality = 0;
	SwapChain.SampleDesc.Count = 1;

	SwapChain.OutputWindow = _hWnd;
	SwapChain.Windowed = static_cast<BOOL>(_isWindowed);
	SwapChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (FAILED(pFactory->CreateSwapChain(m_pDevice, &SwapChain, &m_pSwapChain)))
		return E_FAIL;

	Safe_Release(pFactory);
	Safe_Release(pAdapter);
	Safe_Release(pDevice);

	return S_OK;
}

HRESULT CGraphicDevice::Ready_BackBufferRenderTargetView()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	ID3D11Texture2D* pBackBufferTexture = nullptr;

	if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateRenderTargetView(pBackBufferTexture, nullptr, &m_pBackBufferRTV)))
		return E_FAIL;

	Safe_Release(pBackBufferTexture);

	return S_OK;
}

HRESULT CGraphicDevice::Ready_DepthStencilView(_uint _winWidth, _uint _winHeight)
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	ID3D11Texture2D* pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	/* 깊이 버퍼의 픽셀은 백버퍼의 픽셀과 갯수가 동일해야만 깊이 텍스트가 가능해진다. */
	/* 픽셀의 수가 다르면 아에 렌더링을 못함. */
	TextureDesc.Width = _winWidth;
	TextureDesc.Height = _winHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	/* 동적? 정적?  */
	TextureDesc.Usage = D3D11_USAGE_DEFAULT /* 정적 */;
	/* 추후에 어떤 용도로 바인딩 될 수 있는 View타입의 텍스쳐를 만들기위한 Texture2D입니까? */
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL
		/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	/* RenderTargetView */
	/* ShaderResourceView */
	/* DepthStencilView */

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pDepthStencilView)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

	return S_OK;
}
