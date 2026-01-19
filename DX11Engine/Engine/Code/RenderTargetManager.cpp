#include "epch.h"
#include "RenderTargetManager.h"

CRenderTargetManager::CRenderTargetManager()
    : m_width(0)
    , m_height(0)
    , m_rtList({})
{
}

CRenderTargetManager::~CRenderTargetManager()
{
}

CRenderTargetManager& CRenderTargetManager::GetInstance()
{
    static CRenderTargetManager inst;
    return inst;
}

HRESULT CRenderTargetManager::Initialize()
{
    ID3D11Device* device = CGraphicDevice::GetInstance().Get_Device();

    const int width = CDisplay::GetInstance().Get_ScreenResolution().x;
    const int height = CDisplay::GetInstance().Get_ScreenResolution().y;

    if (!device || width == 0 || height == 0)
        return E_FAIL;

    Destroy();
    return CreateTargets(device, width, height);
}

void CRenderTargetManager::Destroy()
{
    for (auto& kv : m_rtList)
        kv.second.Destroy();

    m_rtList.clear();
    m_width = 0;
    m_height = 0;
}

HRESULT CRenderTargetManager::Resize(ID3D11Device* device, UINT width, UINT height)
{
    if (!device || width == 0 || height == 0)
        return E_FAIL;

    if (width == m_width && height == m_height)
        return S_OK;

    Destroy();
    return CreateTargets(device, static_cast<_uint>(width), static_cast<_uint>(height));
}

void CRenderTargetManager::Bind_RenderTarget(const CRenderTarget::RTType type, ID3D11DeviceContext* context, const D3D11_VIEWPORT* vp)
{
    if (!context)
        return;

    auto it = m_rtList.find(type);
    if (it == m_rtList.end())
        return;

    Unbind_AllSRVs_PS(context);

    CRenderTarget& rt = it->second;

    if (type == CRenderTarget::RTType::Depth)
    {
        ID3D11DepthStencilView* dsv = rt.GetDSV();
        if (!dsv)
            return;

        context->OMSetRenderTargets(0, nullptr, dsv);

        if (vp)
            context->RSSetViewports(1, vp);

        return;
    }

    ID3D11RenderTargetView* rtv = rt.GetRTV();
    if (!rtv)
        return;

    ID3D11DepthStencilView* dsv = nullptr;
    auto itDepth = m_rtList.find(CRenderTarget::RTType::Depth);
    if (itDepth != m_rtList.end())
        dsv = itDepth->second.GetDSV();

    context->OMSetRenderTargets(1, &rtv, dsv);

    if (vp)
        context->RSSetViewports(1, vp);
}

void CRenderTargetManager::Clear_RenderTarget(const CRenderTarget::RTType type)
{
    ID3D11DeviceContext* context = CGraphicDevice::GetInstance().Get_Context();
    if (!context)
        return;

    auto it = m_rtList.find(type);
    if (it == m_rtList.end())
        return;

    CRenderTarget& rt = it->second;

    if (type == CRenderTarget::RTType::Depth)
    {
        ID3D11DepthStencilView* dsv = rt.GetDSV();
        if (!dsv)
            return;

        context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        return;
    }

    float clear[4] = { 0, 0, 0, 0 };

    if (type == CRenderTarget::RTType::Normal)
    {
        clear[0] = 0.5f;
        clear[1] = 0.5f;
        clear[2] = 1.0f;
        clear[3] = 1.0f;
    }

    ID3D11RenderTargetView* rtv = rt.GetRTV();
    if (!rtv)
        return;

    context->ClearRenderTargetView(rtv, clear);
}

ID3D11ShaderResourceView* CRenderTargetManager::GetSRV(const CRenderTarget::RTType type) const
{
    auto it = m_rtList.find(type);

    if (it == m_rtList.end())
        return nullptr;

    return it->second.GetSRV();
}

void CRenderTargetManager::Unbind_AllSRVs_PS(ID3D11DeviceContext* context)
{
    if (!context)
        return;

    ID3D11ShaderResourceView* nullSRV[16] = {};
    context->PSSetShaderResources(0, 16, nullSRV);
}

HRESULT CRenderTargetManager::CreateTargets(ID3D11Device* device, _uint width, _uint height)
{
    m_width = width;
    m_height = height;

    if (FAILED(m_rtList[CRenderTarget::RTType::Albedo] .Create(CRenderTarget::RTType::Albedo, device, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, true)))
        return E_FAIL;

    if (FAILED(m_rtList[CRenderTarget::RTType::Normal].Create(CRenderTarget::RTType::Normal, device, width, height, DXGI_FORMAT_R16G16B16A16_FLOAT, true)))
        return E_FAIL;

    if (FAILED(m_rtList[CRenderTarget::RTType::Depth].Create(CRenderTarget::RTType::Depth, device, width, height, DXGI_FORMAT_D24_UNORM_S8_UINT, true)))
        return E_FAIL;

    if (FAILED(m_rtList[CRenderTarget::RTType::Shading].Create(CRenderTarget::RTType::Shading, device, width, height, DXGI_FORMAT_R16G16B16A16_FLOAT, true)))
        return E_FAIL;

    return S_OK;
}
