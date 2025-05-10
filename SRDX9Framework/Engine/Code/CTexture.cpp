#include "CTexture.h"
#include "EDebug.h"

CTexture::CTexture()
	: m_pTexture(nullptr)
	, m_tDesc({})
	, m_v2Size(vector2Int::zero())
{
}

CTexture::~CTexture()
{
	Release();
}

HRESULT CTexture::Load(LPDIRECT3DDEVICE9 _device)
{
    Release();

    if (m_strFilePath.empty())
        return E_FAIL;

    CDebug::Log(L"[CTexture::Load] File = " + m_strFilePath);

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
    return S_OK;
}

void CTexture::Release()
{
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
		m_v2Size = vector2Int::zero();
		m_bLoaded = false;
	}
}
