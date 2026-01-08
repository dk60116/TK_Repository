#include "epch.h"
#include "ShadowMap.h"

CShadowMap::CShadowMap()
    : m_pTexture(nullptr)
    , m_pDSV(nullptr)
    , m_pSRV(nullptr)
    , m_pCmpSampler(nullptr)
    , m_sVP({})
    , m_iSize(2048)
    , m_fTexelSize({ 1.f / 2048.f, 1.f / 2048.f })
{
    m_strName = L"ShadowMap";
}

CShadowMap::~CShadowMap()
{
    OnDestroy();
}

CShadowMap* CShadowMap::Create(const _uint _size)
{
    CShadowMap* newMap = new CShadowMap();

    if (FAILED(newMap->Initialize(_size)))
    {
        Safe_Release(newMap);
        return nullptr;
    }
    
    return newMap;
}

void CShadowMap::Clear()
{
    if (m_pDSV) 
        CGraphicDevice::Get_Context()->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH, 1.f, 0);
}

void CShadowMap::Bind_DSV()
{
    CGraphicDevice::Get_Context()->OMSetRenderTargets(0, nullptr, m_pDSV);
    CGraphicDevice::Get_Context()->RSSetViewports(1, &m_sVP);
}

void CShadowMap::Bind_SRV(const _uint _slot)
{
    ID3D11ShaderResourceView* srv = m_pSRV;
    CGraphicDevice::Get_Context()->PSSetShaderResources(_slot, 1, &srv);
}

void CShadowMap::Bind_Sampler(_uint _slot)
{
    ID3D11SamplerState* s = m_pCmpSampler;
    CGraphicDevice::Get_Context()->PSSetSamplers(_slot, 1, &s);
}

HRESULT CShadowMap::Initialize(const _uint _size)
{
    auto dev = CGraphicDevice::Get_Device();

    if (!dev)
        return E_FAIL;

    m_iSize = _size;
    m_fTexelSize = { _float(1.f / _size), _float(1.f / _size) };
    DXGI_FORMAT typeless = DXGI_FORMAT_R32_TYPELESS;
    DXGI_FORMAT dsvFmt = DXGI_FORMAT_D32_FLOAT;
    DXGI_FORMAT srvFmt = DXGI_FORMAT_R32_FLOAT;

    D3D11_TEXTURE2D_DESC td = {};
    td.Width = _size;
    td.Height = _size;
    td.MipLevels = 1; td.ArraySize = 1;
    td.Format = typeless;
    td.SampleDesc.Count = 1;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

    if (FAILED(dev->CreateTexture2D(&td, nullptr, &m_pTexture)))
        return E_FAIL;

    D3D11_DEPTH_STENCIL_VIEW_DESC dsv = {};
    dsv.Format = dsvFmt; dsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    if (FAILED(dev->CreateDepthStencilView(m_pTexture, &dsv, &m_pDSV)))
        return E_FAIL;

    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    sampDesc.BorderColor[0] = 1.0f;
    sampDesc.BorderColor[1] = 1.0f;
    sampDesc.BorderColor[2] = 1.0f;
    sampDesc.BorderColor[3] = 1.0f;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    if (FAILED(dev->CreateSamplerState(&sampDesc, &m_pCmpSampler)))
        return E_FAIL;

    D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
    srv.Format = srvFmt; srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srv.Texture2D.MipLevels = 1;
    if (FAILED(dev->CreateShaderResourceView(m_pTexture, &srv, &m_pSRV)))
        return E_FAIL;

    m_sVP = { 0, 0, static_cast<_float>(_size), static_cast<_float>(_size), 0.f, 1.f };

    return S_OK;
}

void CShadowMap::OnDestroy()
{
    Safe_Release(m_pTexture);
    Safe_Release(m_pDSV);
    Safe_Release(m_pSRV);
    Safe_Release(m_pCmpSampler);
}
