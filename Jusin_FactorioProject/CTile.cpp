#include "CTile.h"
#include "CGameObject.h"
#include "CSpriteRenderer.h"
#include "CSceneManager.h"

CTile::CTile()
	: m_eType(TILE_DEFAULT)
	, m_v2Index(vector2Int::zero())
	, m_pRenderer(nullptr)
	, m_pRenderLevel(nullptr)
	, m_iTextureIndex(0)
	, m_pHaveStructure(nullptr)
	, m_pItemSlots()
	, m_bIsItemMove(false)
{
}

CTile::CTile(CTile& _rhs)
	: m_eType(_rhs.m_eType)
	, m_v2Index(_rhs.m_v2Index)
	, m_pRenderer(nullptr)
	, m_pRenderLevel(nullptr)
	, m_iTextureIndex(_rhs.m_iTextureIndex)
	, m_pHaveStructure(nullptr)
	, m_pItemSlots()
	, m_bIsItemMove(false)
{
	for (int i = 0; i < 4; ++i)
		m_pItemSlots[i] = nullptr;
}

CTile::~CTile()
{
	OnDestroy();
}

void CTile::Awake()
{
	m_pGameObject->SetLayer(Layer::Tile);
	m_pRenderer = dynamic_cast<CSpriteRenderer*>(m_pGameObject->AddComponent(new CSpriteRenderer(L"Terrain_Dirt_2", vector2Int::one() * 16.f)));
	m_pRenderLevel = dynamic_cast<CSpriteRender_Level*>(m_pGameObject->AddComponent(new CSpriteRender_Level(m_pRenderer)));
	m_pRenderer->SetSprite(m_strTexture);

	vector2Int start[3];
	start[0] = vector2Int(0, 0);
	start[1] = vector2Int(0, 80);
	start[2] = vector2Int(0, 128);
	vector2Int end[3];
	end[0] = vector2Int(64, 64);
	end[1] = vector2Int(32, 32);
	end[2] = vector2Int(16, 16);

	m_pRenderLevel->SetRenderPos(start, end);
	m_pRenderLevel->SetIsRandomX(true);
	m_pRenderLevel->Awake();

	Awake_();
}

void CTile::Start()
{
	Start_();
}

void CTile::Reset()
{
	Reset_();
}

void CTile::Update()
{
}

void CTile::FixedUpdate()
{
}

void CTile::LateUpdate()
{
}

void CTile::OnEnable()
{
}

void CTile::OnDisable()
{
}

void CTile::Render(HDC _hDC)
{
}

void CTile::OnCollisionEnter(CCollider* _other)
{
}

void CTile::OnCollisionStay(CCollider* _other)
{
}

void CTile::OnCollisionExit(CCollider* _other)
{
}

void CTile::OnDestroy()
{
	OnDestroy_();
}

void CTile::OnMouseEnter()
{
}

void CTile::OnMouseOver()
{
}

void CTile::OnMouseExit()
{
}

void CTile::MessageUpdate()
{
	m_pGameObject->MessageUpdate();
}

void CTile::SetHaveStructure(CStructure* _value)
{
	m_pHaveStructure = _value;
}

int CTile::DropItem(CDropItem* _item)
{
	for (int i = 0; i < 4; ++i)
	{
		if (!m_pItemSlots[i])
		{
			m_pItemSlots[i] = _item;
			return i;
		}
	}

	return -1;
}

void CTile::DropItem(CItem* _item, int _index)
{
	CGameObject* newObj = CSceneManager::GetInstance().getCurrentscene()->AddObject(CItemManager::GetInstance().GetItem(_item->getInfo().id)->getInfo().itemName + L"Drop Image", Layer::FieldItem);
	CDropItem* newDrop = dynamic_cast<CDropItem*>(newObj->AddComponent(new CDropItem(_item->getInfo().id, this, _index)));

	m_pItemSlots[_index] = newDrop;
}

void CTile::SetItem(int _index, CDropItem* _item)
{
	m_pItemSlots[_index] = _item;
}
