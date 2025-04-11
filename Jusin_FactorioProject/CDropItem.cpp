#include "CDropItem.h"
#include "CItemManager.h"
#include "CGameObject.h"
#include "CSceneManager.h"
#include "CTile.h"
#include "CBeltManager.h"
#include "CMapManager.h"
#include "CInput.h"
#include "CPhysics.h"
#include "CCollider_Rect.h"

CDropItem::CDropItem(int _itemId, CTile* _tile)
	: m_eType(FIELD)
	, m_pItem(nullptr)
	, m_pCrtTile(_tile)
	, m_pCrtBelt(nullptr)
	, m_pRender(nullptr)
	, m_pLevel(nullptr)
	, m_iTileIndex(-1)
	, m_bTempMoving(false)
{
	m_pItem = CItemManager::GetInstance().GetItem(_itemId);
}

CDropItem::CDropItem(int _itemId, CTile* _tile, int _index)
	: m_eType(FIELD)
	, m_pItem(nullptr)
	, m_pCrtTile(_tile)
	, m_pCrtBelt(nullptr)
	, m_pRender(nullptr)
	, m_pLevel(nullptr)
	, m_iTileIndex(_index)
	, m_bTempMoving(false)
{
	m_pItem = CItemManager::GetInstance().GetItem(_itemId);
}

CDropItem::CDropItem(int _itemId, CBelt* _belt)
	: m_eType(BELT)
	, m_pItem(nullptr)
	, m_pCrtTile(nullptr)
	, m_pCrtBelt(_belt)
	, m_pRender(nullptr)
	, m_pLevel(nullptr)
	, m_iTileIndex(-1)
	, m_bTempMoving(false)
{
	m_pItem = CItemManager::GetInstance().GetItem(_itemId);
}

CDropItem::~CDropItem()
{
	OnDestroy();
}

void CDropItem::Awake()
{
	Awake_();

	CGameObject* renderObj = m_pGameObject->getScene()->AddObject(L"FieldItemRenderer", Layer::FieldItem);
	m_pRender = dynamic_cast<CSpriteRenderer*>(renderObj->AddComponent(new CSpriteRenderer(m_pItem->getInfo().icon_Slot, vector2Int::one() * 32, true)));
	m_pLevel = dynamic_cast<CSpriteRender_Level*>(renderObj->AddComponent(new CSpriteRender_Level(m_pRender)));
	
	vector2Int start[3];
	vector2Int end[3];

	start[0] = vector2Int(0, 0);
	start[1] = vector2Int(0, 32);
	start[2] = vector2Int(0, 48);
	end[0] = vector2Int::one() * 32;
	end[1] = vector2Int::one() * 16;
	end[2] = vector2Int::one() * 8;

	m_pLevel->SetRenderPos(start, end);

	renderObj->SetParent(m_pGameObject);
	renderObj->getTransform().SetScale(vector2::one() * 0.45f);

	if (m_pCrtTile)
	{
		getTransform().SetPosition(m_pCrtTile->getTransform().getPosition());

		if (m_iTileIndex < 0)
			m_iTileIndex = m_pCrtTile->DropItem(this);

		float offset = 0.25f;

		if (m_iTileIndex == 0)
			getTransform().AddPosition(vector2(offset, offset));
		else if (m_iTileIndex == 1)
			getTransform().AddPosition(vector2(-offset, offset));
		else if (m_iTileIndex == 2)
			getTransform().AddPosition(vector2(-offset, -offset));
		else if (m_iTileIndex == 3)
			getTransform().AddPosition(vector2(offset, -offset));

		if (m_iTileIndex < 0)
			m_pGameObject->Destroy();
	}

	CBeltManager::GetInstance().AddItem(this);
}

void CDropItem::Start()
{
	Start_();
}

void CDropItem::Update()
{
	Update_();

	CheckStayBelt();
	Reset();
}


void CDropItem::LateUpdate()
{
}

void CDropItem::OnMouseEnter()
{
}

void CDropItem::OnMouseExit()
{
}

void CDropItem::OnDestroy()
{
	OnDestroy_();
	
	CBeltManager::GetInstance().DeleteItem(this);
}

void CDropItem::Reset()
{
	if (m_pCrtBelt)
	{
		vector2 beltPos = m_pCrtBelt->getTransform().getPosition();
		float beltSpd = CBeltManager::GetInstance().getBeltSpeed();
		float pivot = 0.3f;
		vector2 myPos = getTransform().getPosition();

		switch (m_pCrtBelt->getIDir())
		{
		case CStructure::RIGHT:
			for (TRAVERSAL_ITER(CBeltManager::GetInstance().getItemList(), it))
			{
				if ((*it) == this || (*it)->m_bTempMoving)
					continue;
				if (vector2::Distance(myPos, (*it)->getTransform().getPosition()) > 1.45f)
					continue;
				if (fabs((*it)->getTransform().getPosition().y - myPos.y) >= pivot)
					continue;
				if ((*it)->getTransform().getPosition().x < myPos.x)
					continue;
				if (fabs((*it)->getTransform().getPosition().x - myPos.x) <= pivot)
					return;
			}

			if (myPos.y < beltPos.y - pivot)
			{
				getTransform().AddPosition(vector2::down() * beltSpd * DELTA_TIME);
				m_bTempMoving = true;
			}
			else if (myPos.y > beltPos.y + pivot)
			{
				getTransform().AddPosition(vector2::up() * beltSpd * DELTA_TIME);
				m_bTempMoving = true;
			}
			else
			{
				getTransform().AddPosition(vector2::right() * beltSpd * DELTA_TIME);
				m_bTempMoving = false;
			}
			break;

		case CStructure::DOWN:
			for (TRAVERSAL_ITER(CBeltManager::GetInstance().getItemList(), it))
			{
				if ((*it) == this || (*it)->m_bTempMoving)
					continue;
				if (vector2::Distance(myPos, (*it)->getTransform().getPosition()) > 1.45f)
					continue;
				if (fabs((*it)->getTransform().getPosition().x - myPos.x) >= pivot)
					continue;
				if ((*it)->getTransform().getPosition().y < myPos.y)
					continue;
				if (fabs((*it)->getTransform().getPosition().y - myPos.y) <= pivot)
					return;
			}

			if (myPos.x < beltPos.x - pivot)
			{
				getTransform().AddPosition(vector2::right() * beltSpd * DELTA_TIME);
				m_bTempMoving = true;
			}
			else if (myPos.x > beltPos.x + pivot)
			{
				getTransform().AddPosition(vector2::left() * beltSpd * DELTA_TIME);
				m_bTempMoving = true;
			}
			else
			{
				getTransform().AddPosition(vector2::down() * beltSpd * DELTA_TIME);
				m_bTempMoving = false;
			}
			break;

		case CStructure::LEFT:
			for (TRAVERSAL_ITER(CBeltManager::GetInstance().getItemList(), it))
			{
				if ((*it) == this)
					continue;
				if (vector2::Distance(myPos, (*it)->getTransform().getPosition()) > 1.45f)
					continue;
				if (fabs((*it)->getTransform().getPosition().y - myPos.y) >= pivot)
					continue;
				if ((*it)->getTransform().getPosition().x > myPos.x)
					continue;
				if (fabs((*it)->getTransform().getPosition().x - myPos.x) < pivot)
					return;
			}

			if (myPos.y < beltPos.y - pivot)
			{
				getTransform().AddPosition(vector2::down() * beltSpd * DELTA_TIME);
				m_bTempMoving = true;
			}
			else if (myPos.y > beltPos.y + pivot)
			{
				getTransform().AddPosition(vector2::up() * beltSpd * DELTA_TIME);
				m_bTempMoving = true;
			}
			else
			{
				getTransform().AddPosition(vector2::left() * beltSpd * DELTA_TIME);
				m_bTempMoving = false;
			}
			break;

		case CStructure::UP:
			for (TRAVERSAL_ITER(CBeltManager::GetInstance().getItemList(), it))
			{
				if ((*it) == this)
					continue;
				if (vector2::Distance(myPos, (*it)->getTransform().getPosition()) > 1.45f)
					continue;
				if (fabs((*it)->getTransform().getPosition().x - myPos.x) >= pivot)
					continue;
				if ((*it)->getTransform().getPosition().y > myPos.y)
					continue;
				if (fabs((*it)->getTransform().getPosition().y - myPos.y) <= pivot)
					return;
			}

			if (myPos.x < beltPos.x - pivot)
				getTransform().AddPosition(vector2::right() * beltSpd * DELTA_TIME);
			else if (myPos.x > beltPos.x + pivot)
				getTransform().AddPosition(vector2::left() * beltSpd * DELTA_TIME);
			else
				getTransform().AddPosition(vector2::up() * beltSpd * DELTA_TIME);
			break;

		case 4:
			for (TRAVERSAL_ITER(CBeltManager::GetInstance().getItemList(), it))
			{
				if ((*it) == this)
					continue;
				if (vector2::Distance(myPos, (*it)->getTransform().getPosition()) > 1.45f)
					continue;
				if (fabs((*it)->getTransform().getPosition().x - myPos.x) >= pivot)
					continue;
				if ((*it)->getTransform().getPosition().y > myPos.y)
					continue;
				if (fabs((*it)->getTransform().getPosition().y - myPos.y) <= pivot)
					return;
			}

			if (myPos.y < beltPos.y && myPos.x > beltPos.x + pivot)
				getTransform().AddPosition(vector2::left () * beltSpd * DELTA_TIME);
			else if (myPos.y > beltPos.y && myPos.x > beltPos.x)
				getTransform().AddPosition(vector2::left() * beltSpd * DELTA_TIME);
			else if (myPos.y > beltPos.y)
				getTransform().AddPosition(vector2(-1.f, -1.f).normalize() * beltSpd * DELTA_TIME);
			else
				getTransform().AddPosition(vector2::up() * beltSpd * DELTA_TIME);
			break;

		case 5:
			for (TRAVERSAL_ITER(CBeltManager::GetInstance().getItemList(), it))
			{
				if ((*it) == this || (*it)->m_bTempMoving)
					continue;
				if (vector2::Distance(myPos, (*it)->getTransform().getPosition()) > 1.45f)
					continue;
				if (fabs((*it)->getTransform().getPosition().x - myPos.x) >= pivot)
					continue;
				if ((*it)->getTransform().getPosition().y < myPos.y)
					continue;
				if (fabs((*it)->getTransform().getPosition().y - myPos.y) <= pivot)
					return;
			}

			if (myPos.x < beltPos.x && myPos.y < beltPos.y)
				getTransform().AddPosition(vector2::down() * beltSpd * DELTA_TIME);
			else if (myPos.x > beltPos.x && myPos.y < beltPos.y - pivot * 2.f)
				getTransform().AddPosition(vector2::down() * beltSpd * DELTA_TIME);
			else if (myPos.y < beltPos.y + pivot)
				getTransform().AddPosition(vector2(1.f, 1.f).normalize() * beltSpd * DELTA_TIME);
			else
				getTransform().AddPosition(vector2::right() * beltSpd * DELTA_TIME);
			break;

		case 6:
			for (TRAVERSAL_ITER(CBeltManager::GetInstance().getItemList(), it))
			{
				if ((*it) == this)
					continue;
				if (vector2::Distance(myPos, (*it)->getTransform().getPosition()) > 1.45f)
					continue;
				if (fabs((*it)->getTransform().getPosition().x - myPos.x) >= pivot)
					continue;
				if ((*it)->getTransform().getPosition().y > myPos.y)
					continue;
				if (fabs((*it)->getTransform().getPosition().y - myPos.y) <= pivot)
					return;
			}

			if (myPos.y > beltPos.y && myPos.x < beltPos.x)
				getTransform().AddPosition(vector2::right() * beltSpd * DELTA_TIME);
			else if (myPos.y < beltPos.y && myPos.x < beltPos.x - pivot)
				getTransform().AddPosition(vector2::right() * beltSpd * DELTA_TIME);
			else if (myPos.y > beltPos.y)
				getTransform().AddPosition(vector2(1.f, -1.f).normalize() * beltSpd * DELTA_TIME);
			else
				getTransform().AddPosition(vector2::up() * beltSpd * DELTA_TIME);
			break;

		case 7:
			for (TRAVERSAL_ITER(CBeltManager::GetInstance().getItemList(), it))
			{
				if ((*it) == this || (*it)->m_bTempMoving)
					continue;
				if (vector2::Distance(myPos, (*it)->getTransform().getPosition()) > 1.45f)
					continue;
				if (fabs((*it)->getTransform().getPosition().x - myPos.x) >= pivot)
					continue;
				if ((*it)->getTransform().getPosition().y < myPos.y)
					continue;
				if (fabs((*it)->getTransform().getPosition().y - myPos.y) <= pivot)
					return;
			}

			if (myPos.x > beltPos.x && myPos.y < beltPos.y)
				getTransform().AddPosition(vector2::down() * beltSpd * DELTA_TIME);
			else if (myPos.y < beltPos.y && myPos.x < beltPos.x - pivot * 2.f)
				getTransform().AddPosition(vector2::down() * beltSpd * DELTA_TIME);
			else if (myPos.y < beltPos.y + pivot)
				getTransform().AddPosition(vector2(-1.f, 1.f).normalize() * beltSpd * DELTA_TIME);
			else
				getTransform().AddPosition(vector2::left() * beltSpd * DELTA_TIME);
			break;

		case 8:
			for (TRAVERSAL_ITER(CBeltManager::GetInstance().getItemList(), it))
			{
				if ((*it) == this)
					continue;
				if (vector2::Distance(myPos, (*it)->getTransform().getPosition()) > 1.45f)
					continue;
				if (fabs((*it)->getTransform().getPosition().x - myPos.x) >= pivot)
					continue;
				if ((*it)->getTransform().getPosition().y > myPos.y)
					continue;
				if (fabs((*it)->getTransform().getPosition().y - myPos.y) <= pivot)
					return;
			}

			if (myPos.x < beltPos.x && myPos.y > beltPos.y + pivot)
				getTransform().AddPosition(vector2::up() * beltSpd * DELTA_TIME);
			else if (myPos.x > beltPos.x && myPos.y > beltPos.y + pivot * 2.f)
				getTransform().AddPosition(vector2::up() * beltSpd * DELTA_TIME);
			else if (myPos.y > beltPos.y - pivot)
				getTransform().AddPosition(vector2(1.f, -1.f).normalize() * beltSpd * DELTA_TIME);
			else
				getTransform().AddPosition(vector2::right() * beltSpd * DELTA_TIME);
			break;

		case 9:
			for (TRAVERSAL_ITER(CBeltManager::GetInstance().getItemList(), it))
			{
				if ((*it) == this)
					continue;
				if (vector2::Distance(myPos, (*it)->getTransform().getPosition()) > 1.45f)
					continue;
				if (fabs((*it)->getTransform().getPosition().y - myPos.y) >= pivot)
					continue;
				if ((*it)->getTransform().getPosition().x > myPos.x)
					continue;
				if (fabs((*it)->getTransform().getPosition().x - myPos.x) < pivot)
					return;
			}

			if (myPos.y < beltPos.y && myPos.x > beltPos.x)
				getTransform().AddPosition(vector2::left() * beltSpd * DELTA_TIME);
			else if (myPos.y > beltPos.y && myPos.x > beltPos.x + pivot * 2.f)
				getTransform().AddPosition(vector2::left () * beltSpd * DELTA_TIME);
			else if (myPos.x > beltPos.x - pivot)
				getTransform().AddPosition(vector2(-1.f, 1.f).normalize() * beltSpd * DELTA_TIME);
			else
				getTransform().AddPosition(vector2::down() * beltSpd * DELTA_TIME);
			break;

		case 10:
			for (TRAVERSAL_ITER(CBeltManager::GetInstance().getItemList(), it))
			{
				if ((*it) == this)
					continue;
				if (vector2::Distance(myPos, (*it)->getTransform().getPosition()) > 1.45f)
					continue;
				if (fabs((*it)->getTransform().getPosition().y - myPos.y) >= pivot)
					continue;
				if ((*it)->getTransform().getPosition().x > myPos.x)
					continue;
				if (fabs((*it)->getTransform().getPosition().x - myPos.x) < pivot)
					return;
			}

			if (myPos.x > beltPos.x && myPos.y > beltPos.y)
				getTransform().AddPosition(vector2::up() * beltSpd * DELTA_TIME);
			//else if (myPos.x > beltPos.x && myPos.x > beltPos.x + pivot * 2.f)
			//	getTransform().AddPosition(vector2::up() * beltSpd * DELTA_TIME);
			else if (myPos.y > beltPos.y - pivot)
				getTransform().AddPosition(vector2(-1.f, -1.f).normalize() * beltSpd * DELTA_TIME);
			else
				getTransform().AddPosition(vector2::left() * beltSpd * DELTA_TIME);
			break;

		case 11:
			for (TRAVERSAL_ITER(CBeltManager::GetInstance().getItemList(), it))
			{
				if ((*it) == this || (*it)->m_bTempMoving)
					continue;
				if (vector2::Distance(myPos, (*it)->getTransform().getPosition()) > 1.45f)
					continue;
				if (fabs((*it)->getTransform().getPosition().x - myPos.x) >= pivot)
					continue;
				if ((*it)->getTransform().getPosition().y < myPos.y)
					continue;
				if (fabs((*it)->getTransform().getPosition().y - myPos.y) <= pivot)
					return;
			}

			if (myPos.y < beltPos.y && myPos.x < beltPos.x - pivot)
				getTransform().AddPosition(vector2::right() * beltSpd * DELTA_TIME);
			//else if (myPos.x > beltPos.x && myPos.x > beltPos.x + pivot * 2.f)
			//	getTransform().AddPosition(vector2::up() * beltSpd * DELTA_TIME);
			else if (myPos.x < beltPos.x + pivot)
				getTransform().AddPosition(vector2(1.f, 1.f).normalize() * beltSpd * DELTA_TIME);
			else
				getTransform().AddPosition(vector2::down() * beltSpd * DELTA_TIME);
			break;
		}
	}
}

void CDropItem::FixedUpdate()
{
}

void CDropItem::OnEnable()
{
}

void CDropItem::OnDisable()
{
}

void CDropItem::Render(HDC _hdc)
{
}

void CDropItem::OnCollisionEnter(CCollider* _other)
{
}

void CDropItem::OnCollisionStay(CCollider* _other)
{
}

void CDropItem::OnCollisionExit(CCollider* _other)
{
}

void CDropItem::OnMouseOver()
{
}

void CDropItem::ChangeTile(CTile* _tile)
{
	m_pCrtTile = _tile;
	_tile->SetItem(m_iTileIndex, this);
}

void CDropItem::CheckStayBelt()
{
	CTileMap& map = *CMapManager::GetInstance().getMainTile();
	vector2 myPos = getTransform().getPosition();

	vector2Int index = vector2Int((int)round(myPos.y), (int)round(myPos.x));

	if (map(index)->getHaveStructure())
	{
		if (map(index)->getHaveStructure()->getObj()->GetComponent<CBelt>())
		{
			m_eType = BELT;
			m_iTileIndex = -1;
			m_pCrtTile = nullptr;
			m_pCrtBelt = map(index)->getHaveStructure()->getObj()->GetComponent<CBelt>();
		}
		else
			m_pCrtBelt = nullptr;
	}
	else
		m_pCrtBelt = nullptr;
}
