#include "epch.h"
#include "Text.h"

CText::CText()
	: m_pFont(nullptr)
	, m_strText(L"")
{
	m_strName = L"Text";
}

CText::~CText()
{
    OnDestroy();
}

CText* CText::Create()
{
    return new CText();
}

HRESULT CText::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

    if (!m_pFont)
        Set_Font(CResources::GetInstance().LoadOnGame<CFont>(L"Sans (Font)"));

	return S_OK;
}

void CText::Render_Editor()
{
}

void CText::Render()
{
    if (!m_pFont || !m_pFont->Get_SpriteFont())
        return;

    SpriteBatch* batch = CGraphicDevice::GetInstance().Get_SpriteBatch();

    // 텍스트 위치 계산 (AnchoredPosition 사용)
    CRectTransform* rect = m_pGameObject->GetComponent<CRectTransform>();

    if (!rect)
        return;

    const vector2 anchoredPos = rect->Get_AnchoredPosition();
    const vector2 scale = rect->Get_WidthHeight();
    const vector2 pivot = rect->Get_Pivot();

    // 출력 위치
    XMFLOAT2 pos;
    pos.x = anchoredPos.x;
    pos.y = anchoredPos.y;

    batch->Begin();

    m_pFont->Get_SpriteFont()->DrawString
    (
        batch,
        m_strText.c_str(),
        pos,
        m_vColor.toXMVector(),
        0.f,
        XMFLOAT2(pivot.x, pivot.y),
        XMFLOAT2(scale.x, scale.y)
    );

    batch->End();
}

void CText::OnDestroy()
{
    Safe_Release(m_pFont);
}

void CText::Set_Font(CFont* _font)
{
    if (m_pFont == _font)
        return;

    if (m_pFont)
        Safe_Release(m_pFont);

    m_pFont = _font;

    if (m_pFont)
        m_pFont->AddRef();
}

void CText::Set_Text(wstring _text)
{
    m_strText = _text;
}

void CText::Set_Text(string _text)
{
    m_strText = CEngineString::StringToWString(_text);
}
