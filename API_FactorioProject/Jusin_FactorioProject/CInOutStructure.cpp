#include "CInOutStructure.h"
#include "CSceneManager.h"
#include "CPlayer.h"
#include "CInventoryUI.h"

CInOutStructure::CInOutStructure()
	: m_pStructure(nullptr)
    , m_eType(ALL)
	, m_bIsTasking(false)
	, m_pInputItem(nullptr)
	, m_pOutputItem(nullptr)
	, m_pFuelItem(nullptr)
	, m_ipInputItemMax(50)
	, m_ipFuelItemMax(5)
	, m_iOutputItemMax(10)
	, m_fTaskProgress(0.f)
	, m_fFuelLife(0.f)
	, m_sTaskOption({})
	, m_bTempFuel(false)
{
}

CInOutStructure::~CInOutStructure()
{
	OnDestroy();
}

void CInOutStructure::Awake()
{
	Awake_();
}

void CInOutStructure::Start()
{
	Start_();
}

void CInOutStructure::Reset()
{
	Reset_();
}

void CInOutStructure::Update()
{
    Update_();

    switch (m_eType)
    {
    case CInOutStructure::ALL:

        if (!m_bIsTasking && m_pInputItem && m_pFuelItem)
        {
            m_bIsTasking = true;
            m_fFuelLife = m_sTaskOption.fuelMax;
            m_pFuelItem->getInfo().count -= 1;
            if (m_pFuelItem->getInfo().count <= 0)
                RetrunFuelItem();
            m_bTempFuel = true;
        }

        if (m_bIsTasking && m_pInputItem && (m_pFuelItem || m_bTempFuel))
        {
            if (m_fTaskProgress < m_sTaskOption.taskMax)
                m_fTaskProgress += m_sTaskOption.taskSpeed * DELTA_TIME;

            if (m_fFuelLife > 0.f)
                m_fFuelLife -= m_sTaskOption.fuelSpeed * DELTA_TIME;
        }

        if (m_bIsTasking && (m_pFuelItem || m_bTempFuel) && m_fTaskProgress >= m_sTaskOption.taskMax)
        {
            m_fTaskProgress = 0.f;

            if (m_pInputItem->getInfo().count <= 0)
            {
                ReturnInputItem();
                m_bTempFuel = true;
            }

            if (!m_pOutputItem)
            {
                int outId = m_sTaskOption.ioItems[IOItem{ m_pInputItem->getInfo().id, 1 }].itemId;
                CItem* newItem = new CItem(*CItemManager::GetInstance().GetItem(outId));
                newItem->getInfo().count = m_sTaskOption.ioItems[IOItem{ m_pInputItem->getInfo().id, 1 }].itemCount;
                m_pOutputItem = newItem;
            }
            else
            {
                m_pOutputItem->getInfo().count += 1;
            }

            CSceneManager::GetInstance().getCurrentscene()
                ->FindObjectOfType<CPlayer>(Layer::Player)
                ->getInventory()->getInvenUI()->OnEnable();

            m_pInputItem->getInfo().count -= m_sTaskOption.ioItems[IOItem{ m_pInputItem->getInfo().id, 1 }].itemCount;

            if (m_pInputItem->getInfo().count <= 0)
                m_pInputItem = nullptr;

            CSceneManager::GetInstance().getCurrentscene()
                ->FindObjectOfType<CPlayer>(Layer::Player)
                ->getInventory()->getInvenUI()->OnEnable();
        }

        if (m_bIsTasking && m_fFuelLife <= 0.f)
        {
            if (m_pFuelItem)
            {
                m_fFuelLife = m_sTaskOption.fuelMax;
                m_pFuelItem->getInfo().count -= 1;
                if (m_pFuelItem->getInfo().count <= 0)
                    RetrunFuelItem();
                CSceneManager::GetInstance().getCurrentscene()
                    ->FindObjectOfType<CPlayer>(Layer::Player)
                    ->getInventory()->getInvenUI()->OnEnable();
            }
            else
            {
                m_bTempFuel = false;
            }
        }

        if (!m_pFuelItem && !m_bTempFuel)
        {
            m_bIsTasking = false;
            m_fTaskProgress = 0.f;
        }
        break;
    case CInOutStructure::INPUT:
        break;
    case CInOutStructure::FUEL:
        if (!m_bIsTasking && m_pFuelItem)
        {
            m_bIsTasking = true;
            m_fFuelLife = m_sTaskOption.fuelMax;
            m_pFuelItem->getInfo().count -= 1;
            if (m_pFuelItem->getInfo().count <= 0)
                RetrunFuelItem();
            m_bTempFuel = true;
        }

        if (m_bIsTasking && (m_pFuelItem || m_bTempFuel))
        {
            if (m_fTaskProgress < m_sTaskOption.taskMax)
                m_fTaskProgress += m_sTaskOption.taskSpeed * DELTA_TIME;

            if (m_fFuelLife > 0.f)
                m_fFuelLife -= m_sTaskOption.fuelSpeed * DELTA_TIME;
        }

        if (m_bIsTasking && (m_pFuelItem || m_bTempFuel) && m_fTaskProgress >= m_sTaskOption.taskMax)
        {
            m_fTaskProgress = 0.f;

            m_bTempFuel = true;

            CSceneManager::GetInstance().getCurrentscene()
                ->FindObjectOfType<CPlayer>(Layer::Player)
                ->getInventory()->getInvenUI()->OnEnable();

            m_pStructure->OnOutputHandler();
        }

        if (m_bIsTasking && m_fFuelLife <= 0.f)
        {
            if (m_pFuelItem)
            {
                m_fFuelLife = m_sTaskOption.fuelMax;
                m_pFuelItem->getInfo().count -= 1;
                if (m_pFuelItem->getInfo().count <= 0)
                    RetrunFuelItem();
                CSceneManager::GetInstance().getCurrentscene()
                    ->FindObjectOfType<CPlayer>(Layer::Player)
                    ->getInventory()->getInvenUI()->OnEnable();
            }
            else
            {
                m_bTempFuel = false;
            }
        }

        if (!m_pFuelItem && !m_bTempFuel)
        {
            m_bIsTasking = false;
            m_fTaskProgress = 0.f;
        }
        break;
    case CInOutStructure::FUEL_OUT:
        break;
    case CInOutStructure::IN_OUT:
        break;
    default:
        break;
    }
}



void CInOutStructure::FixedUpdate()
{
	FixedUpdate_();
}

void CInOutStructure::LateUpdate()
{
	LateUpdate_();
}

void CInOutStructure::OnEnable()
{
	OnEnable_();
}

void CInOutStructure::OnDisable()
{
	OnDisable_();
}

void CInOutStructure::Render(HDC _hDC)
{
}

void CInOutStructure::OnCollisionEnter(CCollider* _other)
{
}

void CInOutStructure::OnCollisionStay(CCollider* _other)
{
}

void CInOutStructure::OnCollisionExit(CCollider* _other)
{
}

void CInOutStructure::OnDestroy()
{
	OnDestroy_();

	Safe_Delete(m_pInputItem);
	Safe_Delete(m_pOutputItem);
	Safe_Delete(m_pFuelItem);
}

void CInOutStructure::OnMouseEnter()
{
}

void CInOutStructure::OnMouseOver()
{
}

void CInOutStructure::OnMouseExit()
{
}

void CInOutStructure::SetInputItem(CItem* _item)
{
    if (m_pInputItem)
    {
        if (m_pInputItem->getInfo().id == _item->getInfo().id)
            ++m_pInputItem->getInfo().count;
    }
    else
    {
        m_pInputItem = _item;
    }
}

void CInOutStructure::AddIOItemPair(const vector2Int _input, const vector2Int _output)
{
	m_sTaskOption.ioItems.insert(make_pair(IOItem{ CItemManager::GetInstance().GetItem(_input.x)->getInfo().id, _input.y }, IOItem{ CItemManager::GetInstance().GetItem(_output.x)->getInfo().id, _output.y }));
}

void CInOutStructure::ReturnInputItem()
{
	Safe_Delete(m_pInputItem);
}

void CInOutStructure::ReturnOutputItem()
{
	Safe_Delete(m_pOutputItem);
}

void CInOutStructure::RetrunFuelItem()
{
	Safe_Delete(m_pFuelItem);
}
