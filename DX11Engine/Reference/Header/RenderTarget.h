#pragma once
#include "epch.h"

NS_BEGIN(Engine)

class ENGINE_DLL CRenderTarget final
{
public:
    enum class RTType
    {
        Albedo,
        Normal,
        Depth,
        Shading
    };

public:
    CRenderTarget();
    ~CRenderTarget();

public:
    HRESULT Create(
        RTType type,
        ID3D11Device* device,
        _uint width,
        _uint height,
        DXGI_FORMAT format,
        bool createSRV = true
    );

    void Destroy();

public:
    RTType GetType() const { return m_type; }
    _uint GetWidth() const { return m_width; }
    _uint GetHeight() const { return m_height; }
    DXGI_FORMAT GetFormat() const { return m_format; }
    bool IsCreateSRV() const { return m_createSRV; }

    ID3D11Texture2D* GetTexture() const { return m_texture; }
    ID3D11RenderTargetView* GetRTV() const { return m_rtv; }              
    ID3D11DepthStencilView* GetDSV() const { return m_dsv; }            
    ID3D11ShaderResourceView* GetSRV() const { return m_srv; }           

    bool IsDepth() const { return m_type == RTType::Depth; }
    bool HasRTV() const { return m_rtv != nullptr; }
    bool HasDSV() const { return m_dsv != nullptr; }
    bool HasSRV() const { return m_srv != nullptr; }

private:
    HRESULT CreateColor_Internal(ID3D11Device* device);
    HRESULT CreateDepth_Internal(ID3D11Device* device);

    static bool GetDepthTypelessFormats(
        DXGI_FORMAT dsvFormat,
        DXGI_FORMAT& outTypelessTexFormat,
        DXGI_FORMAT& outDsvFormat,
        DXGI_FORMAT& outSrvFormat
    );

private:
    RTType m_type;
    _uint m_width;
    _uint m_height;
    DXGI_FORMAT m_format;
    bool m_createSRV;

    ID3D11Texture2D* m_texture;
    ID3D11RenderTargetView* m_rtv;
    ID3D11DepthStencilView* m_dsv;
    ID3D11ShaderResourceView* m_srv;
};

NS_END
