#pragma once
#include "Object.h"

NS_BEGIN(Engine)

class ENGINE_DLL CShadowMap final : public UObject
{
	friend class CCamera;

private:
	explicit CShadowMap();
	~CShadowMap();

public:
	static CShadowMap* Create(const _uint _size = 2048);

	void Clear();
	void Bind_DSV();
	void Bind_SRV(const _uint _slot);
	void Bind_Sampler(_uint _slot);

private:
	HRESULT Initialize(const _uint _size);
	void OnDestroy();

private:
	ID3D11Texture2D* m_pTexture;
	ID3D11DepthStencilView* m_pDSV;
	ID3D11ShaderResourceView* m_pSRV;
	ID3D11SamplerState* m_pCmpSampler;
	D3D11_VIEWPORT m_sVP;
	_uint m_iSize;
	_float2 m_fTexelSize;
};

NS_END