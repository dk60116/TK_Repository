#include "CButton.h"
#include "CGameObject.h"
#include "Jusin_FactorioProject.h"
#include "CInput.h"

extern HWND g_hWnd;

CButton::CButton(wstring _imgName)
	: m_pImage(nullptr)
	, m_strResourceName(_imgName)
	, m_v2SpriteStart(vector2Int::zero())
	, m_v2SpriteEnd(vector2Int::one() * -1.f)
	, m_bHighlight(false)
	, m_bSelect(false)
{
}

CButton::CButton(wstring _imgName, vector2Int _end)
	: m_pImage(nullptr)
	, m_strResourceName(_imgName)
	, m_v2SpriteStart(vector2Int::zero())
	, m_v2SpriteEnd(_end)
	, m_bHighlight(false)
	, m_bSelect(false)
{
}

CButton::~CButton()
{
	OnDestroy();
}

void CButton::Awake()
{
	Awake_();

	m_pImage = dynamic_cast<CImage*>(m_pGameObject->AddComponent(new CImage(m_strResourceName)));
	m_pImage->SetCanvas(m_pCanvas);
	m_pImage->SetSpriteArea(m_v2SpriteStart, m_v2SpriteEnd);
}

void CButton::Start()
{
	Start_();
}

void CButton::Reset()
{
}

void CButton::Update()
{
	Update_();

	m_pImage->getRect().SetPosition(getRect().getPosition());
	m_pImage->getRect().SetSize(getRect().getSize());

	if (m_bHighlight)
		m_pImage->SetSpriteArea(m_v2SpriteStart, m_v2SpriteEnd);
	else
	{
		m_pImage->SetSpriteArea(vector2Int::zero(), m_v2SpriteEnd);
	}

	if (m_bSelect)
	{
		m_pImage->SetSpriteArea(vector2Int(m_v2SpriteStart.x * 2, m_v2SpriteStart.y), m_v2SpriteEnd);

		if (CInput::GetInstance().GetMouseButtonUp(0))
		{
			m_bSelect = false;

			if (m_bHighlight)
				OnClick();
		}
	}

	if (m_bIsMouse)
	{
		m_bHighlight = true;

		if (CInput::GetInstance().GetMouseButtonDown(0))
			m_bSelect = true;
	}
	else
		m_bHighlight = false;
}

void CButton::FixedUpdate()
{
}

void CButton::LateUpdate()
{
	LateUpdate_();

	m_pImage->getObj()->SetActive(m_pGameObject->isActive());
}

void CButton::OnEnable()
{
	OnEnable_();

	m_bHighlight = false;
	m_bSelect = false;
}

void CButton::OnDisable()
{
	OnDisable_();

	m_bHighlight = false;
	m_bSelect = false;
}

void CButton::Render(HDC _hDC)
{
	if (m_pImage->getObj()->isEnable())
		m_pImage->Render(_hDC);
}

void CButton::OnCollisionEnter(CCollider* _other)
{
}

void CButton::OnCollisionStay(CCollider* _other)
{
}

void CButton::OnCollisionExit(CCollider* _other)
{
}

void CButton::OnDestroy()
{
}

void CButton::OnMouseEnter()
{
}

void CButton::OnMouseOver()
{
}

void CButton::OnMouseExit()
{
}

void CButton::AddOnClickEvent(const function<void()>& _callback)
{
	m_vOnClickEvents.push_back(_callback);
}

void CButton::OnClick()
{
	for (auto& cb : m_vOnClickEvents)
		cb();
}

void CButton::SetSpriteArea(vector2Int _start, vector2Int _end)
{
	m_v2SpriteStart = vector2Int(_start);
	m_v2SpriteEnd = vector2Int(_end);
}
