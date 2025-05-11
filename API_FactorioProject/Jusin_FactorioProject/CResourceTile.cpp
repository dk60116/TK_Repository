#include "CResourceTile.h"
#include "CGameObject.h"
#include "CSpriteRenderer.h"
#include "CSceneManager.h"
#include "CTestScene.h"
#include "CRandom.h"

CResourceTile::CResourceTile(ResourceType _type, int _reserve)
	: m_eType(_type)
	, m_iReserves(_reserve)
	, m_fCurrentHp(100)
{
}

CResourceTile::~CResourceTile()
{
	OnDestroy();
}

void CResourceTile::Awake()
{
	Awake_();

	CGameObject* obj = CSceneManager::GetInstance().getCurrentscene()->AddObject(L"", Layer::Ore);
	obj->getTransform().SetScale(vector2::one() * 1.25f);
	obj->SetParent(getObj());
	obj->getTransform().SetChildPosition(vector2(CRandom::GetInstance().Range(-0.25f, 0.25f), CRandom::GetInstance().Range(-0.25f, 0.25f)));

	switch (m_eType)
	{
	case CResourceTile::WATER:
		break;
	case CResourceTile::COAL:
		obj->AddComponent(new CSpriteRenderer(L"Resource_Coal", vector2Int(64, 64), true));
		break;
	case CResourceTile::STONE:
		obj->AddComponent(new CSpriteRenderer(L"Resource_Stone", vector2Int(64, 64), true));
		break;
	case CResourceTile::IRON_ORE:
		obj->AddComponent(new CSpriteRenderer(L"Resource_Iron", vector2Int(64, 64), true));
		break;
	case CResourceTile::COPPER_ORE:
		obj->AddComponent(new CSpriteRenderer(L"Resource_Copper", vector2Int(64, 64), true));
		break;
	default:
		break;
	}

	int random = CRandom::GetInstance().Range(0, 8);
	obj->GetComponent<CSpriteRenderer>()->SetSpriteArea(vector2Int(64 * random, 0), vector2Int::one() * 64);
}

void CResourceTile::Start()
{
	Start_();
}

void CResourceTile::Reset()
{
	Reset_();
}

void CResourceTile::Update()
{
	Update_();
}

void CResourceTile::FixedUpdate()
{
	FixedUpdate_();
}

void CResourceTile::LateUpdate()
{
	LateUpdate_();
}

void CResourceTile::OnEnable()
{
	OnEnable_();
}

void CResourceTile::OnDisable()
{
	OnDisable_();
}

void CResourceTile::Render(HDC _hDC)
{
}

void CResourceTile::OnCollisionEnter(CCollider* _other)
{
}

void CResourceTile::OnCollisionStay(CCollider* _other)
{
}

void CResourceTile::OnCollisionExit(CCollider* _other)
{
}

void CResourceTile::OnDestroy()
{
	OnDestroy_();
}

void CResourceTile::OnMouseEnter()
{
}

void CResourceTile::OnMouseOver()
{
}

void CResourceTile::OnMouseExit()
{
}

void CResourceTile::AddHp(float _value)
{
	m_fCurrentHp += _value;

	if (m_fCurrentHp <= 0)
		GetReward();
}

void CResourceTile::GetReward()
{
	--m_iReserves;
	m_fCurrentHp += 100.f;

	int reward = -1;

	switch (m_eType)
	{
	case CResourceTile::WATER:
		break;
	case CResourceTile::COAL:
		reward = 0;
		break;
	case CResourceTile::STONE:
		reward = 1;
		break;
	case CResourceTile::IRON_ORE:
		reward = 2;
		break;
	case CResourceTile::COPPER_ORE:
		reward = 3;
		break;
	default:
		break;
	}

	dynamic_cast<CTestScene*>(CSceneManager::GetInstance().getCurrentscene())->getPlayer().getInventory()->AddItem(reward, 1);
}
