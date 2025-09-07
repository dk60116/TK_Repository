#pragma once
#include "Object.h"

NS_BEGIN(Engine)

class ENGINE_DLL CRenderTarget final : public UObject
{
private:
	CRenderTarget();
	virtual ~CRenderTarget();

public:
	HRESULT Initialize(const wstring& _name, const vector2Int _size, const DXGI_FORMAT _pixelFormat, const ColorValue _clearColor);
	void Release();

#ifndef _CLIENT_BUILD
public:
	HRESULT Ready_Debug(const vector2 _pos, const _float2 _size);
	HRESULT Render(class CShader* _shader, class CVIBuffer_Rect* _viBuffer);

private:
	_float4x4 m_vWorldMatrix;
#endif

public:
	ID3D11RenderTargetView* Get_RTV() const;

public:
	HRESULT Bind_Shader(const CShader* _shader);
	void Clear();

private:
	wstring m_strTargetName;

	ID3D11RenderTargetView* m_pRTV;
	ID3D11ShaderResourceView* m_pSRV;
	ID3D11Texture2D* m_pTexture2D;

	ColorValue m_vClearColor;
};

NS_END

