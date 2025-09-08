#pragma once
#include "Object.h"

NS_BEGIN(Engine)

class ENGINE_DLL CRenderTarget final : public UObject
{
	friend class CDisplay;

private:
	explicit CRenderTarget();
	virtual ~CRenderTarget();

private:
	static CRenderTarget* Create(const wstring& _name, const vector2Int _pos, const vector2Int _size, const DXGI_FORMAT _pixelFormat, const ColorValue _clearC);

public:
	HRESULT Initialize(const wstring& _name, const vector2Int _pos, const vector2Int _size, const DXGI_FORMAT _pixelFormat, const ColorValue _clearColor);
	void OnDestroy();

public:
	const wstring& Get_RTName();
	ID3D11RenderTargetView* Get_RTV() const;
	ID3D11DepthStencilView* Get_DSV() const;
	const D3D11_VIEWPORT& Get_VP();

private:
	HRESULT Ready_Debug(const vector2Int _pos, const vector2Int _size);

public:
	HRESULT Render();
	HRESULT Bind_Shader();
	void Clear();

private:
	wstring m_strTargetName;

	ID3D11RenderTargetView* m_pRTV;
	ID3D11ShaderResourceView* m_pSRV;
	ID3D11DepthStencilView* m_pDSV;
	ID3D11Texture2D* m_pTexture2D;
	ID3D11Buffer* m_pCBPerMaterial;
	D3D11_VIEWPORT m_sViewPort;

	ColorValue m_vClearColor;

#ifndef _CLIENT_BUILD
	_float4x4 m_vWorldMatrix;

	ID3D11Buffer* m_pCBPerObject;
	ID3D11Buffer* m_pCBPerCamera;

	ID3D11SamplerState* m_pDebugSampler;

	CMeshBuffer* m_pMeshBuffer;
	CShader* m_pDefferdShader;
#endif
};

NS_END

