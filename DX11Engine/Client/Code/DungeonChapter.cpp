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

void CDungeonChapter::OnDestroy()
{
}

void CDungeonChapter::OnTriggerEnter(CCollider* _other)
{
}

void CDungeonChapter::OnTriggerExit(CCollider* _other)
{
}

void CDungeonChapter::SetBoundingBox(const BOUNDINGBOXDESC& _desc)
{
	m_sBoundingBoxInfo = _desc;
}

void CDungeonChapter::OnPlayerEnter()
{
}

void CDungeonChapter::OnPlayerExit()
{
}

void CDungeonChapter::Bind_BoundingBox()
{
	Get_Transform()->Set_Position(m_sBoundingBoxInfo.posScale.first);
	Get_Transform()->Set_LocalScale(m_sBoundingBoxInfo.posScale.second);
	Get_Transform()->Set_EulerAnglesY(m_sBoundingBoxInfo.yRot);
}

void CDungeonChapter::AttachColliders()
{
}
