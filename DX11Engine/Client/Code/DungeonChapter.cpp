#include "cpch.h"
#include "DungeonChapter.h"

CDungeonChapter::CDungeonChapter()
	: m_pDungeon(nullptr)
	, m_pBoundingBox(nullptr)
	, m_vGateList({})
	, m_vMonsterList({})
	, m_vCloneMonsterList({})
	, m_vMapColList({})
{
}

CDungeonChapter::~CDungeonChapter()
{
}

CDungeonChapter* CDungeonChapter::Create()
{
	return new CDungeonChapter();
}

CComponent* CDungeonChapter::Clone() const
{
	return new CDungeonChapter();
}

HRESULT CDungeonChapter::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_pGameObject->SetLayer(L"DungeonChapter");

	if (!m_pBoundingBox)
	{
		m_pBoundingBox = m_pGameObject->AddComponent<CBoxCollider>();
		m_pBoundingBox->Set_GizmoColor(CCollider::GizmoColor::BlackWhite);
		m_pBoundingBox->SetTrigger(true);
	}

	return S_OK;
}

void CDungeonChapter::Awake()
{
	Bind_BoundingBox();
}

void CDungeonChapter::Start()
{
	Spawn_Monsters();
}

void CDungeonChapter::Update()
{
}

void CDungeonChapter::LateUpdate()
{
}

void CDungeonChapter::OnDestroy()
{
	m_vMonsterList.clear();
	
	for (TRAVERSAL_ITER(m_vCloneMonsterList, it))
		Safe_Release((*it));

	m_vCloneMonsterList.clear();
}

void CDungeonChapter::OnTriggerEnter(CCollider* _other)
{
	if (_other->Get_GameObject()->GetTag() == L"Player")
	{
		OnPlayerEnter();
	}
}

void CDungeonChapter::OnTriggerExit(CCollider* _other)
{
	if (_other->Get_GameObject()->GetTag() == L"Player")
	{
		OnPlayerExit();
	}
}

void CDungeonChapter::SetBoundingBox(const BOUNDINGBOXDESC& _desc)
{
	m_sBoundingBoxInfo = _desc;
}

void CDungeonChapter::AddMonsterSpawner(const MONSTERSPAWNER& _desc)
{
	m_vMonsterList.push_back(_desc);
}

void CDungeonChapter::OnPlayerEnter()
{
	AbleColliders();
	AbleMonsters();
}

void CDungeonChapter::OnPlayerExit()
{
	HideColliders();
}

void CDungeonChapter::Bind_BoundingBox()
{
	Get_Transform()->Set_Position(m_sBoundingBoxInfo.posScale.first);
	m_pBoundingBox->Set_Size(m_sBoundingBoxInfo.posScale.second);
	Get_Transform()->Set_EulerAnglesY(m_sBoundingBoxInfo.yRot);
}

void CDungeonChapter::Spawn_Monsters()
{
	for (size_t i = 0; i < m_vMonsterList.size(); ++i)
	{
		auto& spawner = m_vMonsterList[i];

		for (size_t j = 0; j < spawner.position.size(); ++j)
		{
			CGameObject* cloneObj = CGameObject::Instantiate(spawner.prototype->Get_GameObject());
			cloneObj->Get_Transform()->Set_Position(spawner.position[j]);
			CMonster* monster = cloneObj->GetComponent<CMonster>();
			monster->AddRef();
			m_vCloneMonsterList.push_back(monster);
		}
	}
}

void CDungeonChapter::EnableBoundingBox(const _bool _on)
{
	m_pBoundingBox->SetEnabled(_on);
}

void CDungeonChapter::AttachColliders()
{
	auto& colliderList = m_pBoundingBox->Get_EnteredColliders();

	for (TRAVERSAL_ITER(colliderList, it))
	{
		if ((*it).second->Get_GameObject()->GetLayer() == CSceneManager::NameToLayer(L"Map"))
			m_vMapColList.push_back((*it).second);
	}

	HideColliders();
}

void CDungeonChapter::HideColliders()
{
	for (TRAVERSAL_ITER(m_vMapColList, it))
		(*it)->SetEnabled(false);
}

void CDungeonChapter::AbleColliders()
{
	for (TRAVERSAL_ITER(m_vMapColList, it))
		(*it)->SetEnabled(true);
}

void CDungeonChapter::AbleMonsters()
{
	for (TRAVERSAL_ITER(m_vCloneMonsterList, it))
		(*it)->Get_GameObject()->SetActive(true);
}
