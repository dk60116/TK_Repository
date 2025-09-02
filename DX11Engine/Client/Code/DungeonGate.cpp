#include "cpch.h"
#include "DungeonGate.h"

CDungeonGate::CDungeonGate()
	: m_iChapterIndex({})
	, m_bIsOpen(false)
{
}

CDungeonGate::~CDungeonGate()
{
}

CDungeonGate* CDungeonGate::Create()
{
	return new CDungeonGate();
}

CComponent* CDungeonGate::Clone() const
{
	CDungeonGate* clone = new CDungeonGate();

	return clone;
}

HRESULT CDungeonGate::Initialize()
{
	m_strObjName = L"Dungeon_Gate";
	m_bHasAnimation = false;

	m_sDescription.colliderCenter = vector3::up() * 1.5f;
	m_sDescription.colliderSize = vector3(2.f, 3.f, 0.6f);

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_pGameObject->SetLayer(CSceneManager::NameToLayer(L"Map"));

	return S_OK;
}

void CDungeonGate::Awake()
{
	__super::Awake();
}

void CDungeonGate::Update()
{
	__super::Update();

	if (m_bIsOpen)
	{
		CTransform* body = Get_Transform()->Get_Child(0);

		body->Set_LocalPositionY(Lerp(body->Get_LocalPosition().y, -4.f, DELTA_TIME));
		m_pCollider->Set_Center(vector3(0.f, body->Get_LocalPosition().y + 1.5f, 0.f));
	}
}

void CDungeonGate::OnDestroy()
{
	__super::OnDestroy();
}

void CDungeonGate::Open()
{
	m_bIsOpen = true;
}

void CDungeonGate::Close()
{
}
