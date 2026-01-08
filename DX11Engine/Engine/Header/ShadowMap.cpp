#include "epch.h"
#include "ShadowMap.h"

CShadowrMap::CShadowrMap()
    : m_pTexture(nullptr)
    , m_pDSV(nullptr)
    , m_pSRV(nullptr)
    , m_sVP({})
    , m_iSize(2048)
    , m_fTexelSize({ 1.f / 2048.f, 1.f / 2048.f })
{
}

CShadowrMap::~CShadowrMap()
{
    OnDestroy();
}

CShadowrMap* CShadowrMap::Create(ID3D11Device* dev, _uint _size)
{
    m_iSize = size;
    TexelSize = { _float(1.f / size), _float(1.f / size) };
    DXGI_FORMAT typeless = DXGI_FORMAT_R32_TYPELESS;
    DXGI_FORMAT dsvFmt = DXGI_FORMAT_D32_FLOAT;
    DXGI_FORMAT srvFmt = DXGI_FORMAT_R32_FLOAT;

    D3D11_TEXTURE2D_DESC td = {};
    td.Width = size; td.Height = size;
    td.MipLevels = 1; td.ArraySize = 1;
    td.Format = typeless;
    td.SampleDesc.Count = 1;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

    if (FAILED(dev->CreateTexture2D(&td, nullptr, &Tex))) return E_FAIL;

    D3D11_DEPTH_STENCIL_VIEW_DESC dsv = {};
    dsv.Format = dsvFmt; dsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    if (FAILED(dev->CreateDepthStencilView(Tex.Get(), &dsv, &DSV))) return E_FAIL;

    D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
    srv.Format = srvFmt; srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srv.Texture2D.MipLevels = 1;
    if (FAILED(dev->CreateShaderResourceView(Tex.Get(), &srv, &SRV))) return E_FAIL;

    VP = { 0,0, (FLOAT)size, (FLOAT)size, 0.f, 1.f };
    return S_OK;
}

void CShadowrMap::OnDestroy()
{
    Safe_Release(m_pTexture);
    Safe_Release(m_pDSV);
    Safe_Release(m_pSRV);
}
