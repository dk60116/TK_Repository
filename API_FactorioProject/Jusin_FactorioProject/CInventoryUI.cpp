#include "CInventoryUI.h"
#include "CInput.h"
#include "CGameObject.h"
#include "CSceneManager.h"
#include "CCanvas.h"
#include "CImage.h"
#include "CText.h"
#include "CTestScene.h"
#include "CItemSlot.h"
#include "CScreen.h"

CInventoryUI::CInventoryUI()
	: m_pUI(nullptr)
	, m_pInven(nullptr)
	, m_bIsMove(false)
	, m_v2Pivot(vector2::zero())
	, m_pCloseBtn(nullptr)
	, m_vItemSlot({})
	, m_v2ItemSlotSize(vector2Int(5, 5))
	, m_v2CIBtnSize(vector2Int(5, 5))
	, m_pRightSideMenu()
	, m_eSideType(CreateItemTable)
	, m_pIOMenuInputItem(nullptr)
	, m_pOpenedIOStructure(nullptr)
	, m_pIOStructureImage(nullptr)
	, m_pIOMenuOutputItem(nullptr)
	, m_pIOMenuFuelItem(nullptr)
	, m_pIOMenuInputSlot(nullptr)
	, m_pIOMenuOutputSlot(nullptr)
	, m_pIOMenuFuelSlot(nullptr)
	, m_pIOMenuTaskBar(nullptr)
	, m_pIOMenuTaskGauge(nullptr)
	, m_pIOMenuFuelBar(nullptr)
	, m_pIOMenuFuelGauge(nullptr)
{
}

CInventoryUI::~CInventoryUI()
{
}

void CInventoryUI::Awake()
{
	Awake_();

	m_pUI = m_pGameObject->GetComponent<CImage>();
}

void CInventoryUI::Start()
{
	Start_();

	CGameObject* inventoryClosBtn = new CGameObject();
	inventoryClosBtn->SetName(L"InvenCloseBtn");
	inventoryClosBtn->AddComponent(new CButton(L"UI_XButton", vector2Int::one() * 32));
	m_pCloseBtn = inventoryClosBtn->GetComponent<CButton>();
	m_pCloseBtn->SetSpriteArea(vector2Int(32, 0), vector2Int(32, 32));
	m_pCloseBtn->SetParent(m_pUI);
	m_pUI->getCanvs()->InsertUI(inventoryClosBtn->GetComponent<CButton>());
	m_pCloseBtn->getRect().SetSize(vector2(25, 25));
	m_pCloseBtn->getRect().SetAnchorPreset(vector2(1.f, 1.f));
	m_pCloseBtn->getRect().SetChildPos(vector2(-20, 20));
	m_pCloseBtn->AddOnClickEvent([this]()
		{
			m_pGameObject->SetActive(false);
		});

	m_pCloseBtn->UpdateFit();

	for (int i = 0; i < m_v2ItemSlotSize.x * m_v2ItemSlotSize.y; ++i)
	{
		CGameObject* slot = new CGameObject();
		slot->SetLayer(Layer::UI);
		slot->SetName(wstring(L"Inven Slot ") + to_wstring(i));
		slot->AddComponent(new CItemSlot(i));
		CItemSlot& itemSlot = *slot->GetComponent<CItemSlot>();
		itemSlot.SetType(CItemSlot::INVEN);
		itemSlot.SetParent(m_pUI);

		m_vItemSlot.push_back(&itemSlot);

		m_pUI->getCanvs()->InsertUI(&itemSlot);

		itemSlot.getRect().SetSize(vector2::one() * 35.f);
		itemSlot.getRect().SetAnchorPreset(vector2(-1.f, 1.f));
		int cols = m_v2ItemSlotSize.x;

		int row = i / cols;
		int col = i % cols;

		itemSlot.getRect().SetChildPos(vector2(56.f + (54.f * col), 92.f + (52.5f * row)));

		itemSlot.UpdateFit();

		m_pUI->getCanvs()->InsertUI(&itemSlot);
		itemSlot.Start();
	}

	{
		CGameObject* rcm_CreateItem = new CGameObject();
		rcm_CreateItem->SetName(L"RCM_CreateItem");
		rcm_CreateItem->SetLayer(Layer::UI);
		m_pRightSideMenu[CreateItemTable] = dynamic_cast<CImage*>(rcm_CreateItem->AddComponent(new CImage(L"UI_CreateItemMenu")));
		m_pRightSideMenu[CreateItemTable]->SetParent(m_pUI);
		m_pRightSideMenu[CreateItemTable]->SetSpriteArea(vector2Int::zero(), vector2Int::one() * 256);
		m_pRightSideMenu[CreateItemTable]->getRect().SetChildPos(vector2::down() * 20.f);
		m_pRightSideMenu[CreateItemTable]->getRect().SetSize(vector2::one() * 250.f);
		m_pRightSideMenu[CreateItemTable]->getRect().SetAnchorPreset(vector2(0.495f, 0.f));
		m_pRightSideMenu[CreateItemTable]->UpdateFit();
		m_pUI->getCanvs()->InsertUI(m_pRightSideMenu[CreateItemTable]);

		for (int i = 0; i < m_v2CIBtnSize.x * m_v2CIBtnSize.y; ++i)
		{
			CGameObject* btn = new CGameObject();
			btn->SetLayer(Layer::UI);
			btn->SetName(wstring(L"Create Item Btn ") + to_wstring(i));
			btn->AddComponent(new CCreateItemBtn(i));
			CCreateItemBtn& itemBtn = *btn->GetComponent<CCreateItemBtn>();
			itemBtn.SetParent(m_pRightSideMenu[CreateItemTable]);

			m_vCreateItemBtn.push_back(&itemBtn);
			itemBtn.getRect().SetAnchorPreset(vector2(-1.f, 1.f));
			m_pUI->getCanvs()->InsertUI(&itemBtn);

			itemBtn.getRect().SetSize(vector2::zero());
			int cols = m_v2CIBtnSize.x;

			int row = i / cols;
			int col = i % cols;

			itemBtn.getRect().SetChildPos(vector2(30.f + (48.f * col), 30.f + (48.f * row)));

			itemBtn.UpdateFit();

			m_pUI->getCanvs()->InsertUI(&itemBtn);

			itemBtn.Start();
		}

		m_vCreateItemBtn[0]->SetItem(CItemManager::GetInstance().GetItem(4));
		m_vCreateItemBtn[1]->SetItem(CItemManager::GetInstance().GetItem(5));
		m_vCreateItemBtn[5]->SetItem(CItemManager::GetInstance().GetItem(9));
		m_vCreateItemBtn[6]->SetItem(CItemManager::GetInstance().GetItem(10));
		m_vCreateItemBtn[7]->SetItem(CItemManager::GetInstance().GetItem(18));
		m_vCreateItemBtn[10]->SetItem(CItemManager::GetInstance().GetItem(17));
	}
	
	{
		CGameObject* rcm_IOStructure = new CGameObject();
		rcm_IOStructure->SetName(L"RCM_IOStructMenu");
		rcm_IOStructure->SetLayer(Layer::UI);
		m_pRightSideMenu[IOStructureMenu] = dynamic_cast<CImage*>(rcm_IOStructure->AddComponent(new CImage(L"UI_IOStructMenu")));
		m_pRightSideMenu[IOStructureMenu]->SetParent(m_pUI);

		m_pRightSideMenu[IOStructureMenu]->SetSpriteArea(vector2Int::zero(), vector2Int::one() * 256);
		m_pRightSideMenu[IOStructureMenu]->getRect().SetChildPos(vector2::down() * 20.f);
		m_pRightSideMenu[IOStructureMenu]->getRect().SetSize(vector2::one() * 250.f);
		m_pRightSideMenu[IOStructureMenu]->getRect().SetAnchorPreset(vector2(0.495f, 0.f));
		m_pRightSideMenu[IOStructureMenu]->UpdateFit();
		m_pUI->getCanvs()->InsertUI(m_pRightSideMenu[IOStructureMenu]);

		CGameObject* rcm_IO_InputSlot = new CGameObject();
		rcm_IO_InputSlot->SetName(L"RCM_IO_InputSlot");
		rcm_IO_InputSlot->SetLayer(Layer::UI);
		rcm_IO_InputSlot->AddComponent(new CItemSlot(100));
		m_pIOMenuInputSlot = rcm_IO_InputSlot->GetComponent<CItemSlot>();
		m_pIOMenuInputSlot->SetType(CItemSlot::INPUT);
		m_pIOMenuInputSlot->getRect().SetChildPos(vector2(-92.f, 45.f));
		m_pIOMenuInputSlot->SetParent(m_pRightSideMenu[IOStructureMenu]);
		m_pIOMenuInputSlot->UpdateFit();
		m_pUI->getCanvs()->InsertUI(m_pIOMenuInputSlot);
		m_pIOMenuInputSlot->Start();

		CGameObject* rcm_IO_StructureImg = new CGameObject();
		rcm_IO_StructureImg->SetName(L"RCM_IO_StructureImg");
		rcm_IO_StructureImg->SetLayer(Layer::UI);
		m_pIOStructureImage = dynamic_cast<CImage*>(rcm_IO_StructureImg->AddComponent(new CImage(L"")));
		m_pIOStructureImage->SetParent(m_pUI);

		m_pIOStructureImage->SetSpriteArea(vector2Int::zero(), vector2Int::one() * 128);
		m_pIOStructureImage->getRect().SetChildPos(vector2::down() * 60.f);
		m_pIOStructureImage->getRect().SetSize(vector2::one() * 80.f);
		m_pIOStructureImage->getRect().SetAnchorPreset(vector2(0.5f, 0.5f));
		m_pIOStructureImage->UpdateFit();
		m_pUI->getCanvs()->InsertUI(m_pIOStructureImage);

		CGameObject* rcm_IO_OutputSlot = new CGameObject();
		rcm_IO_OutputSlot->SetName(L"RCM_IO_OutputSlot");
		rcm_IO_OutputSlot->SetLayer(Layer::UI);
		rcm_IO_OutputSlot->AddComponent(new CItemSlot(101));
		m_pIOMenuOutputSlot = rcm_IO_OutputSlot->GetComponent<CItemSlot>();
		m_pIOMenuOutputSlot->SetType(CItemSlot::OUTPUT);
		m_pIOMenuOutputSlot->getRect().SetChildPos(vector2(92.f, 45.f));
		m_pIOMenuOutputSlot->SetParent(m_pRightSideMenu[IOStructureMenu]);
		m_pIOMenuOutputSlot->UpdateFit();
		m_pUI->getCanvs()->InsertUI(m_pIOMenuOutputSlot);
		m_pIOMenuOutputSlot->Start();

		CGameObject* rcm_IO_FuelSlot = new CGameObject();
		rcm_IO_FuelSlot->SetName(L"RCM_IO_FuelSlot");
		rcm_IO_FuelSlot->SetLayer(Layer::UI);
		rcm_IO_FuelSlot->AddComponent(new CItemSlot(102));
		m_pIOMenuFuelSlot = rcm_IO_FuelSlot->GetComponent<CItemSlot>();
		m_pIOMenuFuelSlot->SetType(CItemSlot::FUEL);
		m_pIOMenuFuelSlot->getRect().SetChildPos(vector2(-92.f, 93.f));
		m_pIOMenuFuelSlot->SetParent(m_pRightSideMenu[IOStructureMenu]);
		m_pIOMenuFuelSlot->UpdateFit();
		m_pUI->getCanvs()->InsertUI(m_pIOMenuFuelSlot);
		m_pIOMenuFuelSlot->Start();

		CGameObject* rcm_IOTaskGaugeBar = new CGameObject();
		rcm_IOTaskGaugeBar->SetName(L"RCM_IOTaskGaugeBar");
		rcm_IOTaskGaugeBar->SetLayer(Layer::UI);
		m_pIOMenuTaskBar = dynamic_cast<CImage*>(rcm_IOTaskGaugeBar->AddComponent(new CImage(L"UI_GaugeBar_Structure")));
		m_pIOMenuTaskBar->SetSpriteArea(vector2Int::zero(), vector2Int(120, 25));
		m_pIOMenuTaskBar->getRect().SetChildPos(vector2::down() * 45.f);
		m_pIOMenuTaskBar->getRect().SetSize(vector2(120.f, 25.f));
		m_pIOMenuTaskBar->SetColor(ColorValue::Black());
		m_pIOMenuTaskBar->SetParent(m_pRightSideMenu[IOStructureMenu]);
		m_pUI->getCanvs()->InsertUI(m_pIOMenuTaskBar);
		m_pIOMenuTaskBar->UpdateFit();

		CGameObject* rcm_IOTaskGauge = new CGameObject();
		rcm_IOTaskGauge->SetName(L"RCM_IOTaskGauge");
		rcm_IOTaskGauge->SetLayer(Layer::UI);
		m_pIOMenuTaskGauge = dynamic_cast<CImage*>(rcm_IOTaskGauge->AddComponent(new CImage(L"UI_GaugeBar_Green")));
		m_pIOMenuTaskGauge->SetSpriteArea(vector2Int::zero(), vector2Int::one() * 16);
		m_pIOMenuTaskGauge->getRect().SetChildPos(vector2::down() * 45.f);
		m_pIOMenuTaskGauge->getRect().SetSize(vector2(116.f, 21.f));
		m_pIOMenuTaskGauge->SetParent(m_pRightSideMenu[IOStructureMenu]);
		m_pUI->getCanvs()->InsertUI(m_pIOMenuTaskGauge);
		m_pIOMenuTaskGauge->UpdateFit();
		m_pIOMenuTaskGauge->SetFillAmountX(0.f);

		CGameObject* rcm_IOFuelGaugeBar = new CGameObject();
		rcm_IOFuelGaugeBar->SetName(L"RCM_IOTaskGaugeBar");
		rcm_IOFuelGaugeBar->SetLayer(Layer::UI);
		m_pIOMenuFuelBar = dynamic_cast<CImage*>(rcm_IOFuelGaugeBar->AddComponent(new CImage(L"UI_GaugeBar_Fuel")));
		m_pIOMenuFuelBar->SetSpriteArea(vector2Int::zero(), vector2Int(120, 12));
		m_pIOMenuFuelBar->getRect().SetChildPos(vector2::down() * 90.f);
		m_pIOMenuFuelBar->getRect().SetSize(vector2(120.f, 12.f));
		m_pIOMenuFuelBar->SetColor(ColorValue::Black());
		m_pIOMenuFuelBar->SetParent(m_pRightSideMenu[IOStructureMenu]);
		m_pUI->getCanvs()->InsertUI(m_pIOMenuFuelBar);
		m_pIOMenuFuelBar->UpdateFit();
		
		CGameObject* rcm_IOFuelGauge = new CGameObject();
		rcm_IOFuelGauge->SetName(L"RCM_IOFuelGauge");
		rcm_IOFuelGauge->SetLayer(Layer::UI);
		m_pIOMenuFuelGauge = dynamic_cast<CImage*>(rcm_IOFuelGauge->AddComponent(new CImage(L"UI_GaugeBar_Red")));
		m_pIOMenuFuelGauge->SetSpriteArea(vector2Int::zero(), vector2Int::one() * 16);
		m_pIOMenuFuelGauge->getRect().SetChildPos(vector2::down() * 90.f);
		m_pIOMenuFuelGauge->getRect().SetSize(vector2(116.f, 8.f));
		m_pIOMenuFuelGauge->SetParent(m_pRightSideMenu[IOStructureMenu]);
		m_pUI->getCanvs()->InsertUI(m_pIOMenuFuelGauge);
		m_pIOMenuFuelGauge->UpdateFit();
		m_pIOMenuFuelGauge->SetFillAmountX(0.f);
	}

	m_pRightSideMenu[CreateItemTable]->getObj()->SetActive(false);
	m_pRightSideMenu[IOStructureMenu]->getObj()->SetActive(false);

	for (TRAVERSAL_ITER(m_vCreateItemBtn, it))
	{
		if ((*it)->getItem() == nullptr)
			(*it)->Disable();
	}
}

void CInventoryUI::Reset()
{
	Reset_();

	for (TRAVERSAL_ITER(m_vItemSlot, it))
		(*it)->Reset();
}

void CInventoryUI::Update()
{
	Update_();

	if (m_bIsMove && CInput::GetInstance().GetMouseButton(0))
	{
		vector2 mousePos = CInput::GetInstance().GetMousePos().toVector2();
		if (mousePos.x >= CScreen::GetInstance().getOptions().width * 0.025f && mousePos.x < CScreen::GetInstance().getOptions().width * 0.975f
			&& mousePos.y >= CScreen::GetInstance().getOptions().height * 0.025f && mousePos.y < CScreen::GetInstance().getOptions().height * 0.975f)
			m_pUI->getRect().SetPosition(CInput::GetInstance().GetMousePos().toVector2() - m_pUI->getRect().getChildPos());
	}

	if (m_pUI->getOnMouse())
	{
		if (!m_pCloseBtn->getOnMouse())
		{
			if (CInput::GetInstance().GetMouseButtonDown(0))
			{
				if (m_pUI->getMousePivot().y <= -(m_pUI->getRect().getSize().y * 0.4f))
				{
					float y = (float)m_pUI->getMousePivot().y;
					m_bIsMove = true;
					m_pUI->getRect().SetChildPos(m_pUI->getMousePivot().toVector2());
				}
			}
		}
	}

	if (CInput::GetInstance().GetMouseButtonUp(0))
		m_bIsMove = false;

	m_pCloseBtn->UpdateFit();

	if (m_pOpenedIOStructure)
	{
		m_pIOMenuTaskGauge->SetFillAmountX(m_pOpenedIOStructure->getProgress() / m_pOpenedIOStructure->getOption().taskMax);
		m_pIOMenuFuelGauge->SetFillAmountX(m_pOpenedIOStructure->getFuelLife() / m_pOpenedIOStructure->getOption().fuelMax);
	}
}

void CInventoryUI::FixedUpdate()
{
}

void CInventoryUI::LateUpdate()
{
	LateUpdate_();

	m_pIOStructureImage->getObj()->SetActive(m_pRightSideMenu[IOStructureMenu]->getObj()->isActive());
	m_pIOMenuTaskBar->getObj()->SetActive(m_pRightSideMenu[IOStructureMenu]->getObj()->isActive());
	m_pIOMenuTaskGauge->getObj()->SetActive(m_pRightSideMenu[IOStructureMenu]->getObj()->isActive());
	m_pIOMenuFuelBar->getObj()->SetActive(m_pRightSideMenu[IOStructureMenu]->getObj()->isActive());
	m_pIOMenuFuelGauge->getObj()->SetActive(m_pRightSideMenu[IOStructureMenu]->getObj()->isActive());

	for (auto ui : m_pRightSideMenu)
		ui->UpdateFit();
}

void CInventoryUI::OnEnable()
{
	OnEnable_();

	UpdateInventory(m_pInven->getItemList());

	for (int i = 0; i < RightSideMenuType::END; ++i)
	{
		if (m_pRightSideMenu[i]->getObj())
			m_pRightSideMenu[i]->getObj()->SetActive(false);
	}

	UpdateCreateWindow(m_pInven->getItemList());

	m_pIOMenuInputSlot->CloseSlot();
	m_pIOMenuOutputSlot->CloseSlot();
	m_pIOMenuFuelSlot->CloseSlot();

	m_pRightSideMenu[m_eSideType]->getObj()->SetActive(true);

	if (m_pOpenedIOStructure)
	{
		switch (m_pOpenedIOStructure->getType())
		{
		case CInOutStructure::ALL:
			m_pIOMenuInputSlot->AbleSlot();
			m_pIOMenuOutputSlot->AbleSlot();
			m_pIOMenuFuelSlot->AbleSlot();
			break;
		case CInOutStructure::FUEL:
			m_pIOMenuInputSlot->DisableSlot();
			m_pIOMenuOutputSlot->DisableSlot();
			m_pIOMenuFuelSlot->AbleSlot();
			break;
		default:
			break;
		}

		m_pIOStructureImage->SetSprite(m_pOpenedIOStructure->getStructer()->getItem()->getInfo().icon_Field);

		if (m_pOpenedIOStructure->getInputItem())
			m_pIOMenuInputSlot->OpenSlot(m_pOpenedIOStructure->getInputItem());
		if (m_pOpenedIOStructure->getOutputItem())
			m_pIOMenuOutputSlot->OpenSlot(m_pOpenedIOStructure->getOutputItem());
		if (m_pOpenedIOStructure->getFuelItem())
			m_pIOMenuFuelSlot->OpenSlot(m_pOpenedIOStructure->getFuelItem());
	}
}

void CInventoryUI::OnDisable()
{
	OnDisable_();

	m_pIOMenuInputSlot->AbleSlot();
	m_pIOMenuOutputSlot->AbleSlot();
	m_pIOMenuFuelSlot->AbleSlot();

	for (int i = 0; i < RightSideMenuType::END; ++i)
	{
		if (m_pRightSideMenu[i]->getObj())
			m_pRightSideMenu[i]->getObj()->SetActive(false);
	}

	m_pOpenedIOStructure = nullptr;
}

void CInventoryUI::Render(HDC _hDC)
{
}

void CInventoryUI::OnCollisionEnter(CCollider* _other)
{
}

void CInventoryUI::OnCollisionStay(CCollider* _other)
{
}

void CInventoryUI::OnCollisionExit(CCollider* _other)
{
}

void CInventoryUI::OnDestroy()
{
	OnDestroy_();
}

void CInventoryUI::OnMouseEnter()
{
}

void CInventoryUI::OnMouseOver()
{
}

void CInventoryUI::OnMouseExit()
{
}

void CInventoryUI::UpdateInventory(vector<CItem*> itemList)
{
	for (int i = 0; i < itemList.size(); ++i)
	{
		if (itemList[i])
		{
			m_vItemSlot[i]->OpenSlot(itemList[i]);

			if (!CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->isHandOpen())
			{
			}
		}
		else
		{
			m_vItemSlot[i]->CloseSlot();
		}
	}
}

void CInventoryUI::UpdateCreateWindow(vector<CItem*> itemList)
{
	for (int i = 0; i < m_vCreateItemBtn.size(); ++i)
	{
		if (m_vCreateItemBtn[i]->getItem())
		{
			if (m_vCreateItemBtn[i]->getItem()->getInfo().id == 17)
			{
				auto dd = m_vCreateItemBtn[i]->getItem();
				int a = 0;
			}

			NeedItemResult needItems = m_vCreateItemBtn[i]->getItem()->getMaterialsOfItemList(itemList);

			m_vCreateItemBtn[i]->m_bCanCreate = needItems.NeedItems.size() <= 0;

			if (i == 5)
			{
				CItem* item = m_vCreateItemBtn[i]->getItem();
			}
		}
	}
}

void CInventoryUI::AllOffSlot()
{
	for (TRAVERSAL_ITER(m_vItemSlot, it))
		(*it)->m_bIsSelect = false;

	m_pIOMenuInputSlot->m_bIsSelect = false;
	m_pIOMenuOutputSlot->m_bIsSelect = false;
	m_pIOMenuFuelSlot->m_bIsSelect = false;
}


