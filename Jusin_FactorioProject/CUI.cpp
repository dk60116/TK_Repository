#include "CUI.h"
#include "CCanvas.h"
#include "CInput.h"
#include "CGameObject.h"
#include "CScreen.h"

CUI::CUI()
	: m_cRect()
	, m_pCanvas(nullptr)
	, m_bIsMouse(false)
	, m_bIsPrveMouse(false)
	, m_v2MousePivot(vector2Int::zero())
	, m_pParentUI(nullptr)
	, m_lChildList({})
{
}

CUI::~CUI()
{
	OnDestroy_();
}

void CUI::Awake()
{
	Awake_();
}

void CUI::Start()
{
}

void CUI::Reset()
{
}

void CUI::LateUpdate()
{
}

void CUI::OnEnable()
{
	OnEnable_();
}

void CUI::OnDisable()
{
	OnDisable_();
}

void CUI::OnDestroy()
{
}

void CUI::UI_Update()
{
	UpdateFit();

	TRect rect = m_cRect.getTRect();

	m_bIsMouse = false;

	vector2Int mousePos = CInput::GetInstance().GetMousePos();

	if (mousePos.x < m_cRect.getTRect().left)
		return;
	if (mousePos.x > m_cRect.getTRect().right)
		return;
	if (mousePos.y < m_cRect.getTRect().top)
		return;
	if (mousePos.y > m_cRect.getTRect().bottom)
		return;

	m_bIsMouse = true;
	
	vector2Int center((rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2);
	m_v2MousePivot = mousePos - center;

	UI_OnMouseStay();
}

void CUI::UI_LateUpdate()
{
	if (m_bIsMouse && !m_bIsPrveMouse)
		UI_OnMouseEnter();
	else if (!m_bIsMouse && m_bIsPrveMouse)
		UI_OnMouseExit();

	m_bIsPrveMouse = m_bIsMouse;
}

void CUI::UI_OnMouseEnter()
{
}

void CUI::UI_OnMouseStay()
{
}

void CUI::UI_OnMouseExit()
{
}

void CUI::UpdateFit()
{
	if (m_pParentUI != nullptr)
	{
		vector2 parentSize = vector2(m_pParentUI->getRect().getSize().x * getRect().getAnchorPreset().x, -m_pParentUI->getRect().getSize().y * getRect().getAnchorPreset().y);
		m_cRect.SetAnchoredPos(m_pParentUI->m_cRect.getAnchoredPos() + parentSize * 0.5f + m_cRect.getChildPos());
	}
	else
	{
		vector2 screenSize = vector2(CScreen::GetInstance().getOptions().width * 0.5f, CScreen::GetInstance().getOptions().height * 0.5f);
		m_cRect.SetPivot(screenSize * m_cRect.getAnchorPreset());

		for (TRAVERSAL_ITER(m_lChildList, it))
			(*it)->m_cRect.SetPivot(m_cRect.getPivot());
	}
}

void CUI::CanvasInsert(CCanvas* _canvas)
{
	m_pCanvas->InsertUI(this);
}

void CUI::SetParent(CUI* _parent)
{
	if (std::find(_parent->m_lChildList.begin(), _parent->m_lChildList.end(), this) == _parent->m_lChildList.end())
		_parent->m_lChildList.push_back(this);

	m_pParentUI = _parent;
	m_pGameObject->SetParent(_parent->m_pGameObject);
}
