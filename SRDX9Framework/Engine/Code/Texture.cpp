#include "Texture.h"

CTexture::CTexture()
	: m_pTexture(nullptr)
	, m_tDesc({})
	, m_v2Size(vector2Int::zero())
{
}

CTexture::~CTexture()
{
	Destroy();
}

HRESULT CTexture::Load(LPDIRECT3DDEVICE9 _device)
{
    Destroy();

    if (m_strFilePath.empty())
        return E_FAIL;

    if (FAILED(D3DXCreateTextureFromFileEx
    (
        _device,
        m_strFilePath.c_str(),
        D3DX_DEFAULT, D3DX_DEFAULT,
        D3DX_DEFAULT, 0,
        D3DFMT_UNKNOWN,
        D3DPOOL_MANAGED,
        D3DX_DEFAULT, D3DX_DEFAULT,
        0, nullptr, nullptr,
        &m_pTexture
    )))
            return E_FAIL;

    m_pTexture->GetLevelDesc(0, &m_tDesc);
    m_v2Size = vector2Int(static_cast<int>(m_tDesc.Width), static_cast<int>(m_tDesc.Height));

    m_bLoaded = true;

    CDebug::Log(L"[Texture::Load] File = " + m_strFilePath);

    return S_OK;
}

void CTexture::Destroy()
{
    __super::Destroy();

	if (m_pTexture)
	{
        Safe_Release(m_pTexture);
		m_v2Size = vector2Int::zero();
	}
}
