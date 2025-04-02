#include "CInventory.h"
#include "CInventoryUI.h"
#include "CSceneManager.h"
#include "CPlayer.h"

CInventory::CInventory()
	: m_vItemList({})
	, m_pInvenUI(nullptr)
{
}

CInventory::~CInventory()
{
	OnDestroy();
}

void CInventory::Awake()
{
	Awake_();

	m_vItemList.reserve(25);
	m_vItemList.resize(25, nullptr);
	m_vItemList.shrink_to_fit();
}

void CInventory::Start()
{
	Start_();
}

void CInventory::Reset()
{
	Reset_();
}

void CInventory::Update()
{
	Update_();
}

void CInventory::FixedUpdate()
{
	FixedUpdate_();
}

void CInventory::LateUpdate()
{
	LateUpdate_();
}

void CInventory::OnEnable()
{
	OnEnable_();
}

void CInventory::OnDisable()
{
	OnEnable_();
}

void CInventory::Render(HDC _hDC)
{
}

void CInventory::OnCollisionEnter(CCollider* _other)
{
}

void CInventory::OnCollisionStay(CCollider* _other)
{
}

void CInventory::OnCollisionExit(CCollider* _other)
{
}

void CInventory::OnDestroy()
{
	OnDestroy_();

	for (TRAVERSAL_ITER(m_vItemList, it))
		Safe_Delete(*it);

	m_vItemList.clear();
}

void CInventory::OnMouseEnter()
{
}

void CInventory::OnMouseOver()
{
}

void CInventory::OnMouseExit()
{
}

void CInventory::AddItem(const int _itemID, const int _count, const float _fHp)
{
	CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->GetItemHandler();

	int slot = 0;

	for (TRAVERSAL_ITER(m_vItemList, it))
	{
		if ((*it) && (*it)->getInfo().id == _itemID)
		{
			(*it)->getInfo().count += _count;

			m_pInvenUI->UpdateInventory(m_vItemList);
			m_pInvenUI->UpdateCreateWindow(m_vItemList);

#ifdef _DEBUG
			CConsole::Print
			(
				std::string("¾ÆÀÌÅÛ È¹µæ: ") +
				WStringToString(CItemManager::GetInstance().GetItem(_itemID)->getInfo().itemName) +
				std::string(" ") +
				to_string(_count)
			);
#endif // _DEBUG
			return;
		}

		++slot;
	}

	for (TRAVERSAL_ITER(m_vItemList, it))
	{
		if ((*it) == nullptr)
		{
			CItem* item = new CItem(*CItemManager::GetInstance().GetItem(_itemID));
			item->getInfo().count = _count;

			*it = item;

			m_pInvenUI->UpdateInventory(m_vItemList);
			m_pInvenUI->UpdateCreateWindow(m_vItemList);

#ifdef _DEBUG
			CConsole::Print
			(
				std::string("¾ÆÀÌÅÛ È¹µæ: ") +
				WStringToString(CItemManager::GetInstance().GetItem(_itemID)->getInfo().itemName) +
				std::string(" ") +
				to_string(_count)
			);
#endif // _DEBUG

			break;
		}

		++slot;
	}
}

void CInventory::SubItem(const int _itemID, const int _count)
{
	for (TRAVERSAL_ITER(m_vItemList, it))
	{
		if ((*it) && (*it)->getInfo().id == _itemID)
		{
			if ((*it)->getInfo().count > _count)
			{
				(*it)->getInfo().count -= _count;
			}
			else
			{
				//if (CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->getHandItem()->getInfo().id == _itemID)
					//CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->CloseHand();

				int index = static_cast<int>(std::distance(m_vItemList.begin(), it));
				Safe_Delete(*it);

				for (int i = index; i < static_cast<int>(m_vItemList.size()) - 1; ++i)
					m_vItemList[i] = m_vItemList[i + 1];

				m_vItemList[m_vItemList.size() - 1] = nullptr;

				m_pInvenUI->Reset();
			}

			m_pInvenUI->UpdateInventory(m_vItemList);
			m_pInvenUI->UpdateCreateWindow(m_vItemList);

#ifdef _DEBUG
			CConsole::Print
			(
				std::string("¾ÆÀÌÅÛ Á¦°Å: ") +
				WStringToString(CItemManager::GetInstance().GetItem(_itemID)->getInfo().itemName) +
				std::string(" ") +
				to_string(_count)
			);
#endif // _DEBUG

			return;
		}
	}
}

void CInventory::SwapItem(const int _indexA, const int _indexB)
{
	if (_indexA < 0 || _indexA >= static_cast<int>(m_vItemList.size()) ||
		_indexB < 0 || _indexB >= static_cast<int>(m_vItemList.size()) ||
		_indexA == _indexB) 
		return;

	swap(m_vItemList[_indexA], m_vItemList[_indexB]);

	if (m_pInvenUI)
	{
		m_pInvenUI->UpdateInventory(m_vItemList);
		m_pInvenUI->UpdateCreateWindow(m_vItemList);
	}
}

bool CInventory::HaveItem(const int _itemID, const int _count)
{
	for (const auto& item : m_vItemList)
	{
		if (item && item->getInfo().id == _itemID && item->getInfo().count >= _count)
			return true;
	}

	return false;
}
