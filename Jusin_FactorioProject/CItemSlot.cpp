#include "CItemSlot.h"
#include "CGameObject.h"
#include "CInventoryUI.h"
#include "CCanvas.h"
#include "CInput.h"
#include "CSceneManager.h"
#include "CPlayer.h"

bool CItemSlot::m_bInputNext = false;

CItemSlot::CItemSlot(int _index)
	: m_eSlotType(INPUT)
	, m_iIndex(_index)
	, m_pItem(nullptr)
	, m_pSlotImg(nullptr)
	, m_pItemImg(nullptr)
	, m_pCountTxt(nullptr)
	, m_bIsSelect(false)
	, m_bIsSelectNext(false)
	, m_bSwapNext(false)
	, m_bAble(true)
{
}

CItemSlot::~CItemSlot()
{
	OnDestroy();
}

void CItemSlot::UI_OnMouseEnter()
{
	if (!m_pParentUI->getObj()->isActive())
		return;

	CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->SetCrtSlot(this); 
}

void CItemSlot::UI_OnMouseStay()
{
}

void CItemSlot::UI_OnMouseExit()
{
	if (!m_pParentUI->getObj()->isActive())
		return;

	CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->SetCrtSlot(nullptr);
}

void CItemSlot::Awake()
{
	Awake_();
}

void CItemSlot::Start()
{
	Start_();

	CGameObject* slot = new CGameObject();
	m_pSlotImg = dynamic_cast<CImage*>(slot->AddComponent(new CImage(L"UI_InvenSlot")));
	SetDefaultSprite();
	m_pSlotImg->getRect().SetSize(vector2(45, 45));
	m_pSlotImg->SetParent(this);

	CGameObject* item = new CGameObject();
	m_pItemImg = dynamic_cast<CImage*>(item->AddComponent(new CImage(L"")));
	m_pItemImg->SetSpriteArea(vector2Int(0, 0), vector2Int(32, 32));
	m_pItemImg->getRect().SetSize(vector2(35, 35));
	m_pItemImg->SetParent(this);
	m_pItemImg->getObj()->SetActive(false);

	CGameObject* count = new CGameObject();
	m_pCountTxt = dynamic_cast<CText*>(count->AddComponent(new CText()));
	m_pCountTxt->getRect().SetSize(m_pSlotImg->getRect().getSize() * 0.8f);
	m_pCountTxt->SetParent(this);
	m_pCountTxt->getOption().color = ColorValue::White();
	m_pCountTxt->getOption().aligmentX = DT_BOTTOM;
	m_pCountTxt->getOption().aligmentY = DT_RIGHT;

	m_pCanvas->InsertUI(m_pSlotImg);
	m_pCanvas->InsertUI(m_pItemImg);
	m_pCanvas->InsertUI(m_pCountTxt);
}

void CItemSlot::Reset()
{
	Reset_();

	if (!m_pItem || !CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->getInventory()->HaveItem(m_pItem->getInfo().id))
		m_bIsSelect = false;
	SetDefaultSprite();
}

void CItemSlot::Update()
{
	Update_();

	if (!m_pParentUI->getObj()->isActive())
		return;

	if (m_pSlotImg->getOnMouse())
	{
		CItem* handItem = CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->getHandItem();

		if (handItem)
		{
			if (CInput::GetInstance().GetMouseButtonDown(0))
			{
				CItemSlot* targetSlot = CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->getCurrentMouseSlot();

				// 인벤토리 비활성화 후 핸드아이템 인벤토리에 반환
				if (targetSlot && targetSlot->m_eSlotType == INVEN)
				{
					//CInventory& inven = *CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->getInventory();
					//CInventoryUI& invenUI = *inven.getInvenUI();
					//CInOutStructure& io = *invenUI.getIO();

					//inven.AddItem(handItem->getInfo().id, handItem->getInfo().count);

					//if (targetSlot->m_eSlotType == INPUT)
					//{
					//	io.ReturnInputItem();
					//}
					//else if (targetSlot->m_eSlotType == OUTPUT)
					//{
					//	io.ReturnOutputItem();
					//}
					//else if (targetSlot->m_eSlotType == FUEL)
					//{
					//	io.RetrunFuelItem();
					//}

					//CloseSlot();

					//CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->CloseHand();
					//
					//return;
				}
			}

			if (CInput::GetInstance().GetMouseButtonUp(0))
				CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->CloseHand();
		}

		if (!m_bIsSelect)
		{
			SetHighlightSprite();

			if (CInput::GetInstance().GetMouseButtonUp(0))
			{
				m_bSwapNext = false;
			}

			// 아이템 잡기
			CItemSlot* crtSlot = CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->getCurrentMouseSlot();

			if (m_pItem != nullptr && CInput::GetInstance().GetMouseButtonDown(0) && !CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->isHandOpen())
			{
				if (m_eSlotType != INVEN && (crtSlot && crtSlot->m_eSlotType == INVEN))
					return;

				CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->CloseHand();

				m_bIsSelect = true;
				m_bIsSelectNext = false;
				m_pCountTxt->getObj()->SetActive(false);

				return;
			}

			if (CInput::GetInstance().GetMouseButton(0))
				SetHighlightClckSprite();
		}
	}
	else
		SetDefaultSprite();

	if (m_bIsSelect)
	{
		if (!m_pItem)
			return;

		CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->OpenHandle(m_pItem);
		CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->OpenBlueprint();
		CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->BlueprintUpdate();

		CUI* pui = nullptr;

		if (m_eSlotType == INVEN)
			pui = m_pParentUI;
		else
			pui = m_pParentUI->getParent();

		if (!pui->getOnMouse())
		{
			CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->SetCanCreateStructure(true);

			if (m_pItem->getInfo().isStructure)
			{
				CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->AbleHideHandle(false);
			}
			else
			{
				CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->HideBlueprint();
			}
		}
		else
		{
			CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->SetCanCreateStructure(false);
			CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->HideBlueprint();
		}

		if (CInput::GetInstance().GetMouseButtonUp(0))
			m_bIsSelectNext = true;

		m_pItemImg->getObj()->SetActive(false);

		if (m_pSlotImg->getOnMouse())
		{
			SetSpriteHand();

			if (CInput::GetInstance().GetMouseButtonDown(0) && m_bIsSelectNext)
			{
				// 아이템 잡은 후 같은 칸에 다시 놓기
				CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->CloseHand();
				SetDefaultSprite();
				m_pItemImg->getObj()->SetActive(true);
				m_pCountTxt->getObj()->SetActive(true);
				m_bIsSelect = false;
				CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->getInventory()->getInvenUI()->OnEnable();
				return;
			}
		}
		else
		{
			SetSpriteHandHighlight();

			if (CInput::GetInstance().GetMouseButtonDown(0))
			{
				CItemSlot* targetSlot = CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->getCurrentMouseSlot();

				if (targetSlot)
				{
					if (targetSlot->m_eSlotType == INPUT || targetSlot->m_eSlotType == FUEL)
					{
						CItem& handItem = *CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->getHandItem();

						// 재료, 자원에 아이템 입력
						if (&handItem)
						{
							CItem* newItem = new CItem(handItem);
							newItem->getInfo().slot = targetSlot;
							int count = handItem.getInfo().count;
							if (targetSlot->m_pItem)
								count += targetSlot->m_pItem->getInfo().count;
							newItem->getInfo().count = count;
							CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->getInventory()->SubItem(newItem->getInfo().id, newItem->getInfo().count);
							targetSlot->OpenSlot(newItem);
							m_bInputNext = true;
							m_bIsSelect = false;
							m_pItem = nullptr;

							CInventoryUI& invenUI = *CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->getInventory()->getInvenUI();
							CInOutStructure& io = *invenUI.getIO();

							if (targetSlot->m_eSlotType == INPUT)
								io.SetInputItem(newItem);
							else if (targetSlot->m_eSlotType == OUTPUT)
								io.SetOutputItem(newItem);
							else if (targetSlot->m_eSlotType == FUEL)
								io.SetFuelItem(newItem);

							m_bIsSelect = false;
							targetSlot->m_bIsSelect = false;

							CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->getInventory()->getInvenUI()->OnEnable();

							return;
						}
					}
				}

				if (targetSlot && targetSlot->m_pItem && targetSlot != this && m_bIsSelectNext)
				{
					// 아이템 스왑
					CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->CloseHand();
					CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->getInventory()->SwapItem(m_iIndex, targetSlot->m_iIndex);
					CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->OpenHandle(m_pItem);
					CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->OpenBlueprint();
					targetSlot->m_bSwapNext = true;
					CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->getInventory()->getInvenUI()->OnEnable();
					return;
				}
				else
				{
					if (targetSlot && !targetSlot->m_pItem)
					{
						if (m_eSlotType == INVEN && targetSlot->m_eSlotType == INVEN)
						{
							// 아이템 원위치
							CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->CloseHand();
							SetDefaultSprite();
							m_pItemImg->getObj()->SetActive(true);
							m_pCountTxt->getObj()->SetActive(true);
							m_bIsSelect = false;
							CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->getInventory()->getInvenUI()->OnEnable();
							return;
						}
						else if (m_eSlotType != INVEN && targetSlot->m_eSlotType == INVEN)
						{
							// 인벤토리에 반환
							CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->getInventory()->AddItem(m_pItem->getInfo().id, m_pItem->getInfo().count);
							CloseSlot();

							CInventory& inven = *CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->getInventory();
							CInventoryUI& invenUI = *inven.getInvenUI();
							CInOutStructure& io = *invenUI.getIO();

							if (m_eSlotType == INPUT)
							{
								io.ReturnInputItem();
							}
							else if (m_eSlotType == OUTPUT)
							{
								io.ReturnOutputItem();
							}
							else if (m_eSlotType == FUEL)
							{
								io.RetrunFuelItem();
							}

							CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->CloseHand();
							CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->getInventory()->getInvenUI()->OnEnable();
							return;
						}
					}
				}
			}
		}
	}
	else
	{
		if (m_pItem)
		{
			SetDefaultSprite();
			m_pItemImg->getObj()->SetActive(true);

			if (m_pSlotImg->getOnMouse())
				SetHighlightClckSprite();
		}
	}

	if (!m_bInputNext && CInput::GetInstance().GetMouseButtonDown(0))
	{
		if (m_eSlotType == INPUT || m_eSlotType == FUEL)
		{
			if (m_bIsSelect && m_pItem)
			{
				CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->OpenHandle(m_pItem);
				CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->OpenBlueprint();
				return;
			}
		}
	}
}

void CItemSlot::FixedUpdate()
{
	FixedUpdate_();
}

void CItemSlot::LateUpdate()
{
	LateUpdate_();

	if (m_bAble)
		m_pSlotImg->getObj()->SetActive(m_pParentUI->getObj()->isActive());
}

void CItemSlot::OnEnable()
{
	OnEnable_();

	m_pSlotImg->getObj()->SetActive(true);

	if (m_pParentUI->getObj()->isActive() && m_pItem)
	{
		m_pItemImg->getObj()->SetActive(true);
		m_pCountTxt->getObj()->SetActive(true);
	}
}

void CItemSlot::OnDisable()
{
	OnDisable_();

	m_pSlotImg->getObj()->SetActive(false);
	m_pItemImg->getObj()->SetActive(false);
	m_pCountTxt->getObj()->SetActive(false);

	CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->SetCanCreateStructure(true);
	SetDefaultSprite();
}

void CItemSlot::Render(HDC _hDC)
{
}

void CItemSlot::OnCollisionEnter(CCollider* _other)
{
}

void CItemSlot::OnCollisionStay(CCollider* _other)
{
}

void CItemSlot::OnCollisionExit(CCollider* _other)
{
}

void CItemSlot::OnDestroy()
{
	OnDestroy_();
}

void CItemSlot::OnMouseEnter()
{
}

void CItemSlot::OnMouseOver()
{
}

void CItemSlot::OnMouseExit()
{
}

void CItemSlot::OpenSlot(CItem* _item)
{
	m_pItem = _item;
	m_pItemImg->SetSprite(_item->getInfo().icon_Slot);
	m_pCountTxt->SetText(to_wstring(_item->getInfo().count));
	m_pCountTxt->getObj()->SetActive(true);
	m_pItemImg->getObj()->SetActive(true);
}

void CItemSlot::CloseSlot()
{
	m_bIsSelect = false;
	m_pItem = nullptr;
	m_pItemImg->SetSprite(L"");
	m_pCountTxt->SetText(L"");
	m_pItemImg->getObj()->SetActive(false);
}

void CItemSlot::AbleSlot()
{
	m_bAble = true;
	m_pGameObject->SetActive(true);
}

void CItemSlot::DisableSlot()
{
	m_bAble = false;
	m_pGameObject->SetActive(false);
	m_pSlotImg->getObj()->SetActive(false);
}

void CItemSlot::SetDefaultSprite()
{
	switch (m_eSlotType)
	{
	case CItemSlot::INVEN:
	case CItemSlot::INPUT:
	case CItemSlot::QUICKSLOT:
	case CItemSlot::OUTPUT:
		m_pSlotImg->SetSpriteArea(vector2Int(0, 0), vector2Int(32, 32));
		break;
	case CItemSlot::FUEL:
		m_pSlotImg->SetSpriteArea(vector2Int(160, 0), vector2Int(32, 32));
	default:
		break;
	}
}

void CItemSlot::SetHighlightSprite()
{
	switch (m_eSlotType)
	{
	case CItemSlot::INVEN:
	case CItemSlot::INPUT:
	case CItemSlot::QUICKSLOT:
	case CItemSlot::OUTPUT:
		m_pSlotImg->SetSpriteArea(vector2Int(32, 0), vector2Int(32, 32));
		break;
	case CItemSlot::FUEL:
		m_pSlotImg->SetSpriteArea(vector2Int(192, 0), vector2Int(32, 32));
	default:
		break;
	}
}

void CItemSlot::SetHighlightClckSprite()
{
	switch (m_eSlotType)
	{
	case CItemSlot::INVEN:
	case CItemSlot::INPUT:
	case CItemSlot::QUICKSLOT:
	case CItemSlot::OUTPUT:
		m_pSlotImg->SetSpriteArea(vector2Int(64, 0), vector2Int(32, 32));
		break;
	case CItemSlot::FUEL:
		m_pSlotImg->SetSpriteArea(vector2Int(224, 0), vector2Int(32, 32));
	default:
		break;
	}
}

void CItemSlot::SetSpriteHand()
{
	switch (m_eSlotType)
	{
	case CItemSlot::INVEN:
	case CItemSlot::QUICKSLOT:
		m_pSlotImg->SetSpriteArea(vector2Int(96, 0), vector2Int(32, 32));
	case CItemSlot::INPUT:
	case CItemSlot::OUTPUT:
		break;
	case CItemSlot::FUEL:
		m_pSlotImg->SetSpriteArea(vector2Int(32, 0), vector2Int(32, 32));
	default:
		break;
	}
}

void CItemSlot::SetSpriteHandHighlight()
{
	switch (m_eSlotType)
	{
	case CItemSlot::INVEN:
	case CItemSlot::QUICKSLOT:
		m_pSlotImg->SetSpriteArea(vector2Int(128, 0), vector2Int(32, 32));
	case CItemSlot::INPUT:
	case CItemSlot::OUTPUT:
		break;
	case CItemSlot::FUEL:
		m_pSlotImg->SetSpriteArea(vector2Int(32, 0), vector2Int(32, 32));
	default:
		break;
	}
}
