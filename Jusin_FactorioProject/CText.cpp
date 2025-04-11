#include "CText.h"
#include "CGameObject.h"
#include "CScreen.h"

CText::CText()
	: m_strText(L"")
	, m_sOption({})
{
}

CText::~CText()
{
	OnDestroy();
}

void CText::Awake()
{
	Awake_();
}

void CText::Start()
{
	Start_();
}

void CText::Reset()
{
	Reset_();
}

void CText::Update()
{
	Update_();
}

void CText::FixedUpdate()
{
}

void CText::LateUpdate()
{
	LateUpdate_();
}

void CText::OnEnable()
{
	OnEnable_();
}

void CText::OnDisable()
{
	OnDisable_();
}

void CText::Render(HDC _hDC)
{
	if (!m_pCanvas)
		return;

	int bold = m_sOption.bold ? FW_BOLD : FW_NORMAL;

	HFONT hFont = CreateFont
	(
		m_sOption.fontSize, 0, 0, 0, bold, m_sOption.italic, m_sOption.underLint, m_sOption.centerLint,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, m_sOption.font.c_str()
	);

	HFONT hOldFont = (HFONT)SelectObject(_hDC, hFont);

	vector2 pos = m_cRect.getAnchoredPos() + CScreen::GetInstance().getScreenVector().toVector2() * 0.5f;

	RECT rect = { pos.x - (float)getRect().getSize().x * 0.5f, pos.y - (float)getRect().getSize().y * 0.5f, pos.x + (float)getRect().getSize().x * 0.5f, pos.y + (float)getRect().getSize().y * 0.5f };

	SetBkMode(_hDC, TRANSPARENT);

	SetTextColor(_hDC, RGB(m_sOption.color.r, m_sOption.color.g, m_sOption.color.b));

	DrawText(_hDC, m_strText.c_str(), -1, &rect, 
		m_sOption.aligmentX | m_sOption.aligmentY | m_sOption.lineOption);

	SelectObject(_hDC, hOldFont);
	DeleteObject(hFont);
}

void CText::OnCollisionEnter(CCollider* _other)
{
}

void CText::OnCollisionStay(CCollider* _other)
{
}

void CText::OnCollisionExit(CCollider* _other)
{
}

void CText::OnDestroy()
{
	OnDestroy_();
}

void CText::OnMouseEnter()
{
}

void CText::OnMouseOver()
{
}

void CText::OnMouseExit()
{
}

void CText::SetText(wstring _text)
{
	m_strText = _text;
}
