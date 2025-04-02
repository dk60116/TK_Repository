#include "CBurnerDrill.h"
#include "CGameObject.h"
#include "CSceneManager.h"
#include "CMapManager.h"
#include "CResourceTile.h"
#include "CStoneFurnace.h"

CBurnerDrill::CBurnerDrill()
	: m_pInOut(nullptr)
{
}

CBurnerDrill::~CBurnerDrill()
{
	OnDestroy();
}

void CBurnerDrill::Awake()
{
	Awake_();
}

void CBurnerDrill::Start()
{
	Start_();

	StructureStart();

	m_pInOut = dynamic_cast<CInOutStructure*>(m_pGameObject->AddComponent(new CInOutStructure()));
	m_pInOut->SetStructure(this);
	m_pInOut->getOption().taskSpeed = 5.f;
	m_pInOut->getOption().fuelSpeed = 2.f;
	m_pInOut->SetType(CInOutStructure::FUEL);

	CGameObject* animObj = m_pGameObject->getScene()->AddObject(L"CStoneFurnace_FireAnim", Layer::Structure);
	animObj->AddComponent(new CAnimator);
	m_pAnimator = animObj->GetComponent<CAnimator>();
	m_pAnimator->SetRenderer(m_pRenderer);
	m_pRenderer->SetMode(SpriteRenderMode::BMPAnimation);
	m_pRenderer->SetAnimator(m_pAnimator);
	animObj->SetParent(m_pGameObject);
	animObj->getTransform().SetChildPosition(vector2::down() * 0.5f);
	animObj->getTransform().SetScale(vector2(0.3f, 0.4f));

	m_pAnimator->CreateAnimation(L"Play_Down", L"BurnerDrill_Down", 28, vector2::zero(), vector2Int(87, 87), 0, 0.05f);
}

void CBurnerDrill::Reset()
{
	Reset_();
}

void CBurnerDrill::Update()
{
	Update_();

	m_pAnimator->SetEnable(m_pInOut->getFuelItem());
}

void CBurnerDrill::FixedUpdate()
{
}

void CBurnerDrill::LateUpdate()
{
	LateUpdate_();
}

void CBurnerDrill::OnEnable()
{
	OnEnable_();
}

void CBurnerDrill::OnDisable()
{
	OnDisable_();
}

void CBurnerDrill::Render(HDC _hDC)
{
}

void CBurnerDrill::OnCollisionEnter(CCollider* _other)
{
}

void CBurnerDrill::OnCollisionStay(CCollider* _other)
{
}

void CBurnerDrill::OnCollisionExit(CCollider* _other)
{
}

void CBurnerDrill::OnDestroy()
{
	OnDestroy_();
}

void CBurnerDrill::OnMouseEnter()
{
}

void CBurnerDrill::OnMouseOver()
{
}

void CBurnerDrill::OnMouseExit()
{
}

void CBurnerDrill::OnOutputHandler()
{
	CTileMap& map = *CMapManager::GetInstance().getMainTile();

	CTile* targetTile = map(vector2Int(m_vTile[3]->getIndex().y + 1, m_vTile[3]->getIndex().x));

	int item = -1;

	auto targetResource = m_vTile[3]->getObj()->GetComponent<CResourceTile>()->getType();;

	int i = 0;

	if (m_vTile[3] && m_vTile[3]->getObj()->GetComponent<CResourceTile>())
	{
		switch (targetResource)
		{
		case CResourceTile::COAL:
			item = 0;
			break;
		case CResourceTile::STONE:
			item = 1;
			break;
		case CResourceTile::IRON_ORE:
			item = 2;
			break;
		case CResourceTile::COPPER_ORE:
			item = 3;
			break;
		default:
			break;
		}

		if (item < 0)
			return;

		if (targetTile && targetTile->getHaveStructure())
		{
			if (dynamic_cast<CStoneFurnace*>(targetTile->getHaveStructure()))
			{
				CInOutStructure* io = dynamic_cast<CStoneFurnace*>(targetTile->getHaveStructure())->getIO();
				auto resultItem = new CItem(*CItemManager::GetInstance().GetItem(item));
				resultItem->getInfo().count = 1;
				io->SetInputItem(resultItem);
			}
			else
				targetTile->DropItem(CItemManager::GetInstance().GetItem(item), 2);
		}
		else
			targetTile->DropItem(CItemManager::GetInstance().GetItem(item), 2);
	}
}
