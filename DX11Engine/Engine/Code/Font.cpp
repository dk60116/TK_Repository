#include "epch.h"
#include "Font.h"

CFont::CFont()
{
}

CFont::~CFont()
{
    OnDestroy();
}

HRESULT CFont::Initialize(const wstring& _name, const wstring& _filePath, void* _desc)
{
    if (FAILED(__super::Initialize(_name, _filePath, _desc)))
        return E_FAIL;

    ID3D11Device* device = CGraphicDevice::GetInstance().Get_Device();
    ID3D11DeviceContext* context = CGraphicDevice::GetInstance().Get_Context();

    m_pSpriteFont = new SpriteFont(device, _filePath.c_str());

    return S_OK;
}

void CFont::OnDestroy()
{
    if (m_pSpriteFont)
    {
        delete m_pSpriteFont;
        m_pSpriteFont = nullptr;
    }
}
