#include "epch.h"
#include "Texture.h"

CTexture::CTexture()
	: m_pSRV(nullptr)
{
}

CTexture::~CTexture()
{
	OnDestroy();
}

CTexture* CTexture::Create(const wstring& filePath)
{
	return new CTexture();
}

void CTexture::OnDestroy()
{
	__super::Release();

	m_pSRV.Reset();
}

HRESULT CTexture::Initialize(const wstring& _filePath)
{
	if (FAILED(__super::Initialize(_filePath)))
		return E_FAIL;

	ID3D11Device* device = CGraphicDevice::GetInstance().Get_Device();

	if (!device)
		return E_FAIL;

	if (FAILED(CreateWICTextureFromFile(device, m_strFilePath.c_str(), nullptr, m_pSRV.GetAddressOf())))
		return E_FAIL;
	
	return S_OK;
}

ID3D11ShaderResourceView* CTexture::Get_SRV() const
{
	return m_pSRV.Get();
}
