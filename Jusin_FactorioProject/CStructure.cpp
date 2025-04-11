#include "CStructure.h"
#include "CMapManager.h"
#include "CSceneManager.h"
#include "CPlayer.h"
#include "CPhysics.h"
#include "CCollider_Rect.h"

CStructure::CStructure()
	: m_pItem(nullptr)
	, m_vTile({})
	, m_pRenderer(nullptr)
	, m_pAnimator(nullptr)
	, m_fCollectingGauge(100.f)
	, m_pCollider(nullptr)
	, m_eDirType(NONE)
	, m_eDir(UP)
	, m_iDir(0)
{
}

CStructure::~CStructure()
{
	OnDestroy_();
}

void CStructure::StructureStart()
{
	wstring objName = wstring(L"Structure_") + m_pItem->getInfo().itemName;
	CGameObject* rendererObj = CSceneManager::GetInstance().getCurrentscene()->AddObject(objName, Layer::Structure);
	m_pGameObject->getTransform().SetPosition(m_vTile[0]->getTransform().getPosition() - vector2(m_pItem->getInfo().fieldSize.x / 2, m_pItem->getInfo().fieldSize.y / 2) + vector2(m_pItem->getInfo().fieldSize.x, m_pItem->getInfo().fieldSize.y));
	m_pGameObject->getTransform().SetPosition(m_pGameObject->getTransform().getPosition() - vector2::one() * 0.5f);
	rendererObj->getTransform().SetScale(m_pItem->getInfo().fieldSize);
	m_pRenderer = dynamic_cast<CSpriteRenderer*>( rendererObj->AddComponent(new CSpriteRenderer(m_pItem->getInfo().icon_Field, m_pItem->getInfo().fieldSize.toVector2Int() * 64, true)));
	rendererObj->SetParent(m_pGameObject);

	CTileMap& map = *CMapManager::GetInstance().getMainTile();

	vector<vector2Int> tileList = {};
	vector2Int startPos = m_vTile[0]->getIndex();

	CGameObject* colliderObj = CSceneManager::GetInstance().getCurrentscene()->AddObject(objName + L"_Collider", Layer::Structure);
	m_pCollider = dynamic_cast<CCollider*>(colliderObj->AddComponent(new CCollider_Rect(vector2::one())));
	m_pCollider->SetBody(m_pGameObject);
	CPhysics::GetInstance().AddCollider(m_pCollider, Layer::Structure);
	colliderObj->SetParent(m_pGameObject);

	for (int x = startPos.x; x < startPos.x + (int)m_pItem->getInfo().fieldSize.x; ++x)
	{
		for (int y = startPos.y; y < startPos.y + (int)m_pItem->getInfo().fieldSize.y; ++y)
		{
			if (x < map.getRectSize().x, y < map.getRectSize().y)
			{
				tileList.push_back(vector2Int(x, y));
				map(x, y)->SetHaveStructure(this);
			}
		}
	}

#ifdef _DEBUG
	CConsole::Print("Install: " + WStringToString(rendererObj->getName()));
	for (const auto& tile : tileList)
		CConsole::Print("Tile: (" + to_string(tile.x) + ", " + to_string(tile.y) + ")");
#endif // _DEBUG
}

void CStructure::Awake()
{
	Awake_();
}

void CStructure::Start()
{
	Start_();
}

void CStructure::Reset()
{
	Reset_();
}

void CStructure::Update()
{
	Update_();
}

void CStructure::FixedUpdate()
{
	FixedUpdate_();
}

void CStructure::LateUpdate()
{
	LateUpdate_();
}

void CStructure::OnEnable()
{
	OnEnable_();
}

void CStructure::OnDisable()
{
	OnDisable_();
}

void CStructure::Render(HDC _hDC)
{
}

void CStructure::OnCollisionEnter(CCollider* _other)
{
}

void CStructure::OnCollisionStay(CCollider* _other)
{
}

void CStructure::OnCollisionExit(CCollider* _other)
{
}

void CStructure::OnDestroy()
{
	OnDestroy_();

	m_vTile.clear();
}

void CStructure::OnMouseEnter()
{
}

void CStructure::OnMouseOver()
{
}

void CStructure::OnMouseExit()
{
}

void CStructure::SetTile(CTile* _tile)
{
	m_vTile.push_back(_tile);
}

void CStructure::AddCollectingHp(float _value)
{
	m_fCollectingGauge += _value;

	if (m_fCollectingGauge <= 0)
		GetItem();
}

void CStructure::GetItem()
{
	m_fCollectingGauge = 100.f;
	CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->getInventory()->AddItem(m_pItem->getInfo().id, 1);
	
	for (TRAVERSAL_ITER(m_vTile, it))
		(*it)->SetHaveStructure(nullptr);

	m_pGameObject->Destroy();
}