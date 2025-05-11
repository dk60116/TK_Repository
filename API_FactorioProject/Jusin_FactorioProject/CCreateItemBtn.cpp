#include "CCreateItemBtn.h"
#include "CCanvas.h"
#include "CGameObject.h"
#include "CItemManager.h"
#include "CInput.h"
#include "CSceneManager.h"
#include "CPlayer.h"

CCreateItemBtn::CCreateItemBtn(int _index)
	: m_iIndex(_index)
	, m_pItem(nullptr)
	, m_pSlotImg(nullptr)
	, m_pItemImg(nullptr)
	, m_bIsSelect(false)
	, m_bCanCreate(false)
{
}

CCreateItemBtn::~CCreateItemBtn()
{
	OnDestroy();
}

void CCreateItemBtn::UI_OnMouseEnter()
{
	m_bIsSelect = true;
}

void CCreateItemBtn::UI_OnMouseStay()
{
}

void CCreateItemBtn::UI_OnMouseExit()
{
	m_bIsSelect = false;
}

void CCreateItemBtn::Awake()
{
	Awake_();
}

void CCreateItemBtn::Start()
{
	Start_();

	CGameObject* slot = new CGameObject();
	m_pSlotImg = dynamic_cast<CImage*>(slot->AddComponent(new CImage(L"UI_CreateItemBtn")));
	m_pSlotImg->SetSpriteArea(vector2Int(0, 0), vector2Int(32, 32));
	m_pSlotImg->getRect().SetSize(vector2(45, 45));
	m_pSlotImg->SetParent(this);

	CGameObject* item = new CGameObject();
	m_pItemImg = dynamic_cast<CImage*>(item->AddComponent(new CImage(L"")));
	m_pItemImg->SetSpriteArea(vector2Int(0, 0), vector2Int(32, 32));
	m_pItemImg->getRect().SetSize(vector2(35, 35));
	m_pItemImg->SetParent(this);

	m_pCanvas->InsertUI(m_pSlotImg);
	m_pCanvas->InsertUI(m_pItemImg);
}

void CCreateItemBtn::Reset()
{
	Reset_();
}

void CCreateItemBtn::Update()
{
	Update_();

	if (!m_pItem)
	{
		m_pSlotImg->getObj()->SetActive(false);
		return;
	}

	m_pSlotImg->getObj()->SetActive(true);
	
	m_bIsSelect = m_pSlotImg->getOnMouse();

	if (m_bIsSelect)
	{
		m_pSlotImg->SetSpriteArea(vector2Int(64, 0), vector2Int(64, 64));

		if (CInput::GetInstance().GetMouseButton(0))
			m_pSlotImg->SetSpriteArea(vector2Int(128, 0), vector2Int(64, 64));
	}
	else
		m_pSlotImg->SetSpriteArea(vector2Int::zero(), vector2Int(64, 64));

	if (m_pGameObject->isActive() && m_bCanCreate)
	{
		m_pItemImg->SetAlpha(1.f);

		if (m_bIsSelect &&CInput::GetInstance().GetMouseButtonDown(0))
		{
			CPlayer& player = *CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player);
			player.AddCreateItemQueue(m_pItem);
		}
	}
	else
		m_pItemImg->SetAlpha(0.5f);

	m_pSlotImg->getObj()->SetActive(m_pParentUI->getObj()->isActive());
}

void CCreateItemBtn::FixedUpdate()
{
	FixedUpdate_();
}

void CCreateItemBtn::LateUpdate()
{
	LateUpdate_();

	m_pItemImg->getObj()->SetActive(m_pItem && m_pParentUI->getObj()->isActive());
}

void CCreateItemBtn::OnEnable()
{
	OnEnable_();

	if (m_pItem)
	{
		m_pSlotImg->getObj()->SetActive(true);
		m_pItemImg->getObj()->SetActive(true);
	}
}

void CCreateItemBtn::OnDisable()
{
	OnDisable_();

	m_pSlotImg->getObj()->SetActive(false);
	m_pItemImg->getObj()->SetActive(false);
}

void CCreateItemBtn::Render(HDC _hDC)
{
}

void CCreateItemBtn::OnCollisionEnter(CCollider* _other)
{
}

void CCreateItemBtn::OnCollisionStay(CCollider* _other)
{
}

void CCreateItemBtn::OnCollisionExit(CCollider* _other)
{
}

void CCreateItemBtn::OnDestroy()
{
	OnDestroy_();
}

void CCreateItemBtn::OnMouseEnter()
{
}

void CCreateItemBtn::OnMouseOver()
{
}

void CCreateItemBtn::OnMouseExit()
{
}

void CCreateItemBtn::SetItem(CItem* _item)
{
	m_pItem = _item;
	m_pItemImg->SetSprite(_item->getInfo().icon_Slot);
}

void CCreateItemBtn::Disable()
{
	m_pSlotImg->getObj()->SetActive(false);
	m_pItemImg->getObj()->SetActive(false);
}
