#include "cpch.h"
#include "DungeonChest.h"

CDungeonChest::CDungeonChest()
	: m_strItemName(L"")
	, m_iItemCount(1)
	, m_bIsOpen(false)
	, m_bTakeItem(false)
	, m_pJoint(nullptr)
	, m_bDetacted(false)
	, m_fOpenTimer(0.f)
{
}

CDungeonChest::~CDungeonChest()
{
}

CDungeonChest* CDungeonChest::Create()
{
	return new CDungeonChest();
}

CComponent* CDungeonChest::Clone() const
{
	CDungeonChest* clone = new CDungeonChest();

	return clone;
}

HRESULT CDungeonChest::Initialize(void* _desc)
{
	m_strObjName = L"Dungeon_Chest";

	m_sDescription.scaleFactor = 0.009f;
	m_sDescription.colliderCenter = vector3::up() * 0.6f;
	m_sDescription.colliderSize = vector3(2.f, 1.2f, 1.8f);

	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	m_pJoint = m_pGameObject->Get_Scene()->Add_GameObject(L"Joint")->Get_Transform();
	m_pJoint->SetParent(Get_Transform());
	m_pJoint->Set_LocalPosition(0.f, 0.46f, 0.365f);

	return S_OK;
}

void CDungeonChest::Awake()
{
	__super::Awake();

	m_pCollider->SetTrigger(true);
	Get_Transform()->Get_Child(1)->SetParent(m_pJoint);
	m_pJoint->Set_LocalEulerAnglesX(-45.f);

	m_pBodyCollider = Get_Transform()->Get_Child(1)->Get_GameObject()->AddComponent<CBoxCollider>();
	m_pBodyCollider->Get_GameObject()->SetLayer(L"Map");
	m_pBodyCollider->Set_Center(vector3::up() * 0.4f * (1.f / m_sDescription.scaleFactor));
	m_pBodyCollider->Set_Size(vector3(1.f, 0.8f, 0.8f) * (1.f / m_sDescription.scaleFactor));
}

void CDungeonChest::Start()
{
	__super::Start();
}

void CDungeonChest::Update()
{
	__super::Update();

	if (m_bDetacted && !m_bIsOpen)
	{
		if (CInput::GetKeyDown(E))
			Open();
	}

	if (m_bIsOpen)
	{
		m_fOpenTimer += DELTA_TIME;
		m_pJoint->Set_LocalEulerAnglesX(Lerp(m_pJoint->Get_LocalEulerAngles().x, 0.f, DELTA_TIME * 4.f));
	}

	if (m_fOpenTimer > 1.f && !m_bTakeItem)
	{
		CGameManager::GetInstance().Get_Inventory()->AddItem(m_strItemName, m_iItemCount);
		m_bTakeItem = true;

		CGameManager::GetInstance().Get_PlayerHUD()->OpenNotifaction(CGameManager::GetInstance().Get_Inventory()->FindItem(m_strItemName));
	}
}

void CDungeonChest::OnTriggerEnter(CCollider* _other)
{
	if (_other->Get_GameObject()->CompareTag(L"Player"))
		m_bDetacted = true;
}

void CDungeonChest::OnTriggerExit(CCollider* _other)
{
	if (_other->Get_GameObject()->CompareTag(L"Player"))
		m_bDetacted = false;
}

void CDungeonChest::OnDestroy()
{
	__super::OnDestroy();
}

void CDungeonChest::Set_Item(const wstring& _name, _uint _count)
{
	m_strItemName = _name;
	m_iItemCount = _count;
}

void CDungeonChest::Open()
{
	m_bIsOpen = true;
}
