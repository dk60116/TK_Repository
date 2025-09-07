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
	static CRenderTarget* Create(const wstring& _name, const vector2Int _size, const DXGI_FORMAT _pixelFormat, const ColorValue _clearC);

public:
	HRESULT Initialize(const wstring& _name, const vector2Int _size, const DXGI_FORMAT _pixelFormat, const ColorValue _clearColor);
	void OnDestroy();

public:
	const wstring& Get_RTName();

public:
	HRESULT Ready_Debug(const vector2 _pos, const _float2 _size);
	HRESULT Render(class CShader* _shader, class CVIBuffer_Rect* _viBuffer);

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

#ifndef _CLIENT_BUILD
	_float4x4 m_vWorldMatrix;

	ID3D11Buffer* m_pCBPerObject;
	ID3D11Buffer* m_pCBPerCamera;

	ID3D11SamplerState* m_pDebugSampler;

	CMeshBuffer* m_pMeshBuffer;
#endif
};

NS_END

