#include "epch.h"
#include "RenderTarget.h"

CRenderTarget::CRenderTarget()
	: m_strTargetName(L"")
	, m_pRTV(nullptr)
	, m_pSRV(nullptr)
	, m_pTexture2D(nullptr)
	, m_vWorldMatrix()
{
	m_strName = L"Render Target";
}

CRenderTarget::~CRenderTarget()
{
	Release();
}

HRESULT CRenderTarget::Initialize(const wstring& _name, const vector2Int _size, const DXGI_FORMAT _pixelFormat, const ColorValue _clearColor)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};

	textureDesc.Width = _size.x;
	textureDesc.Height = _size.y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = _pixelFormat;

	textureDesc.SampleDesc.Quality = 0;
	textureDesc.SampleDesc.Count = 1;

	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ID3D11Device* device = CGraphicDevice::Get_Device();

	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &m_pTexture2D)))
		return E_FAIL;

	if (FAILED(device->CreateRenderTargetView(m_pTexture2D, nullptr, &m_pRTV)))
		return E_FAIL;

	if (FAILED(device->CreateShaderResourceView(m_pTexture2D, nullptr, &m_pSRV)))
		return E_FAIL;

	m_strName = _name;
	m_vClearColor = _clearColor;

	return S_OK;
}

void CRenderTarget::Release()
{
	Safe_Release(m_pSRV);
	Safe_Release(m_pRTV);
	Safe_Release(m_pTexture2D);
}

#ifndef _CLIENT_BUILD
HRESULT CRenderTarget::Ready_Debug(const vector2 _pos, const _float2 _size)
{
	_uint numViewports = 1;
	D3D11_VIEWPORT viewportDesc = {};

	CGraphicDevice::Get_Context()->RSGetViewports(&numViewports, &viewportDesc);

	XMStoreFloat4x4(&m_vWorldMatrix, XMMatrixScaling(_size.x, _size.y, 1.f));

	m_vWorldMatrix._41 = _pos.x - viewportDesc.Width * 0.5f;
	m_vWorldMatrix._42 = -_pos.y + viewportDesc.Height * 0.5f;

	return S_OK;
}
#endif

HRESULT CRenderTarget::Render(CShader* _shader, CVIBuffer_Rect* _viBuffer)
{
	//_shader->Bind();

	return S_OK;
}

ID3D11RenderTargetView* CRenderTarget::Get_RTV() const
{
	return m_pRTV;
}

HRESULT CRenderTarget::Bind_Shader(const CShader* _shader)
{
	return S_OK;
}
