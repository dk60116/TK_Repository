#include "CCreatingSlot.h"
#include "CGameObject.h"
#include "CSceneManager.h"
#include "CCanvas.h"
#include "CPlayer.h"

CCreatingSlot::CCreatingSlot(int _index)
    : m_iIndex(_index)
    , m_pItem(nullptr)
    , m_pSlotImg(nullptr)
    , m_pItemImg(nullptr)
    , m_pFillImg(nullptr)
    , m_bStart(false)
    , m_fDuring(0.f)
{
}

CCreatingSlot::~CCreatingSlot()
{
    OnDestroy();
}

void CCreatingSlot::UI_OnMouseEnter()
{
}

void CCreatingSlot::UI_OnMouseStay()
{
}

void CCreatingSlot::UI_OnMouseExit()
{
}

void CCreatingSlot::Awake()
{
	Awake_();
}

void CCreatingSlot::Start()
{
	Start_();

    CGameObject* creatingItemUI = new CGameObject();
    creatingItemUI->SetLayer(Layer::UI);
    creatingItemUI->SetName(L"creatingItemUI" + to_wstring(m_iIndex));
    m_pSlotImg = (dynamic_cast<CImage*>(creatingItemUI->AddComponent(new CImage(L"UI_CreateItemWait"))));
    m_pSlotImg->SetSpriteArea(vector2Int::zero(), vector2Int::one() * 64);
    m_pSlotImg->SetAlpha(0.7f);
    m_pSlotImg->SetParent(this);
    m_pSlotImg->getRect().SetSize(vector2::one() * 40);

    CGameObject* creatingItemIcon = new CGameObject();
    creatingItemIcon->SetLayer(Layer::UI);
    creatingItemIcon->SetName(L"creatingItemIcon" + to_wstring(m_iIndex));
    m_pItemImg = (dynamic_cast<CImage*>(creatingItemIcon->AddComponent(new CImage(L""))));
    m_pItemImg->SetSpriteArea(vector2Int::zero(), vector2Int::one() * 32);
    m_pItemImg->SetParent(this);
    m_pItemImg->getRect().SetSize(vector2::one() * 30);

    CGameObject* creatingFill = new CGameObject();
    creatingFill->SetLayer(Layer::UI);
    creatingFill->SetName(L"creatingFill" + to_wstring(m_iIndex));
    m_pFillImg = (dynamic_cast<CImage*>(creatingFill->AddComponent(new CImage(L"UI_CreateItemFill"))));
    m_pFillImg->SetSpriteArea(vector2Int::zero(), vector2Int::one() * 64);
    m_pFillImg->SetAlpha(0.5f);
    m_pFillImg->SetFillAmountY(0.f);
    m_pFillImg->SetParent(this);
    m_pFillImg->getRect().SetSize(vector2::one() * 40);

    m_pCanvas->InsertUI(m_pSlotImg);
    m_pCanvas->InsertUI(m_pItemImg);
    m_pCanvas->InsertUI(m_pFillImg);
}

void CCreatingSlot::Reset()
{
	Reset_();
}

void CCreatingSlot::Update()
{
	Update_();

    if (!m_pItem)
        return;

    if (m_bStart && m_pItem && m_pItem->getInfo().createDuring > m_fDuring)
    {
        m_fDuring += DELTA_TIME;
        
        m_pFillImg->SetFillAmountY(m_fDuring / m_pItem->getInfo().createDuring);
    }

    if (m_bStart && m_pItem && m_pItem->getInfo().createDuring <= m_fDuring)
    {
        m_fDuring = 0.f;
        m_pFillImg->SetFillAmountY(0.f);

        CPlayer& player = *CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player);
        player.getInventory()->AddItem(m_pItem->getInfo().id, m_pItem->getInfo().createCount);
        player.getQueue().pop();

        m_pItem = nullptr;
        m_bStart = false;

        getObj()->SetActive(false);
    }
}

void CCreatingSlot::FixedUpdate()
{
	FixedUpdate_();
}

void CCreatingSlot::LateUpdate()
{
	LateUpdate_();
}

void CCreatingSlot::OnEnable()
{
	OnEnable_();
}

void CCreatingSlot::OnDisable()
{
	OnDisable_();
}

void CCreatingSlot::Render(HDC _hDC)
{
}

void CCreatingSlot::OnCollisionEnter(CCollider* _other)
{
}

void CCreatingSlot::OnCollisionStay(CCollider* _other)
{
}

void CCreatingSlot::OnCollisionExit(CCollider* _other)
{
}

void CCreatingSlot::OnDestroy()
{
}

void CCreatingSlot::OnMouseEnter()
{
}

void CCreatingSlot::OnMouseOver()
{
}

void CCreatingSlot::OnMouseExit()
{
}

void CCreatingSlot::SetItem(CItem& _item)
{
    m_pItem = &_item;
    m_pItemImg->SetSprite(_item.getInfo().icon_Slot);
}
