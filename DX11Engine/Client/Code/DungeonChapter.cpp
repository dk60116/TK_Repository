#include "cpch.h"
#include "DungeonChapter.h"

CDungeonChapter::CDungeonChapter()
	: m_pDungeon(nullptr)
	, m_pBoundingBox(nullptr)
	, m_vGateList({})
	, m_vMonsterList({})
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
}

void CDungeonChapter::Update()
{
}

void CDungeonChapter::LateUpdate()
{
}

void CDungeonChapter::OnDestroy()
{
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

void CDungeonChapter::OnPlayerEnter()
{
	AbleColliders();
}

void CDungeonChapter::OnPlayerExit()
{
	HideColliders();
}

void CDungeonChapter::Bind_BoundingBox()
{
	Get_Transform()->Set_Position(m_sBoundingBoxInfo.posScale.first);
	Get_Transform()->Set_LocalScale(m_sBoundingBoxInfo.posScale.second);
	Get_Transform()->Set_EulerAnglesY(m_sBoundingBoxInfo.yRot);
}

void CDungeonChapter::AttachColliders()
{
	auto& colliderList = m_pBoundingBox->Get_EnteredColliders();

	for (TRAVERSAL_ITER(colliderList, it))
	{
		if ((*it).second->Get_GameObject()->GetLayer() == CSceneManager::NameToLayer(L"Map"))
			m_vMapColList.push_back((*it).second);
	}
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
