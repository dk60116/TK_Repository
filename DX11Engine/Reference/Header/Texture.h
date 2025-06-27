#pragma once

#include "EngineResource.h"

NS_BEGIN(Engine)

class ENGINE_DLL CTexture : public CEngineResource
{
	friend class CResources;

private:
	explicit CTexture();
	~CTexture();

private:
	static CTexture* Create(const wstring& filePath);
	HRESULT Initialize(const wstring& _filePath) override;
	void OnDestroy() override;

public:
	ID3D11ShaderResourceView* Get_SRV() const;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pSRV;
};

NS_END

