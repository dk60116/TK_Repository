#include "cpch.h"
#include "DungeonGate.h"

CDungeonGate::CDungeonGate()
	: m_pBody(nullptr)
	, m_pBodyCollider(nullptr)
	, m_bLock(false)
	, m_bIsOpen(false)
	, m_pLockRenderer(nullptr)
	, m_vLockChainRenders({})
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

	m_sDescription.isTrigger = true;
	m_sDescription.colliderCenter = vector3::up() * 1.5f;
	m_sDescription.colliderSize = vector3(2.f, 3.f, 2.f);

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_pGameObject->SetLayer(CSceneManager::NameToLayer(L"Map"));

	return S_OK;
}

void CDungeonGate::Awake()
{
	__super::Awake();

	if (!m_pBody)
	{
		m_pBody = Get_Transform()->Get_Child();
		m_pBody->Get_GameObject()->SetLayer(L"Map");
	}

	if (!m_pBodyCollider)
	{
		m_pBodyCollider = m_pBody->Get_GameObject()->AddComponent<CBoxCollider>();

		m_pBodyCollider->Set_Center(vector3::up() * 1.5f * (1.f / m_sDescription.scaleFactor));
		m_pBodyCollider->Set_Size(vector3(3.f, 3.f, 0.6f) * (1.f / m_sDescription.scaleFactor));
	}

	if (!m_pLockRenderer)
	{
		CGameObject* lockObj = m_pGameObject->Get_Scene()->Add_GameObject(L"Lock");
		lockObj->Get_Transform()->SetParent(Get_Transform());
		m_pLockRenderer = lockObj->CreateMeshHierachy(CResources::LoadMeshBuffersOnScene(L"Dungeon_Lock_Model (MeshBuffer)"), 0.005f)[0];
		lockObj->Get_Transform()->Set_LocalPositionY(1.7f);
	}
}

void CDungeonGate::Start()
{
	if (m_bLock)
	{
		for (_uint i = 0; i < 3; ++i)
		{
			CGameObject* chainObj = m_pGameObject->Get_Scene()->Add_GameObject(L"Chain" + to_wstring(i));
			m_vLockChainRenders.push_back(chainObj->CreateMeshHierachy(CResources::LoadMeshBuffersOnScene(L"Dungeon_Lock_Chain (MeshBuffer)"), 0.005f)[0]->Get_Transform()->Get_Parent());
			chainObj->Get_Transform()->SetParent(Get_Transform());
			chainObj->Get_Transform()->Set_LocalPosition(vector3::zero());
			chainObj->Get_Transform()->Set_LocalScale(2.5f);
		}

		m_vLockChainRenders[0]->Set_LocalPosition(vector3(-0.25f, 1.8f, 0.f));
		m_vLockChainRenders[0]->Set_LocalEulerAngles(vector3(0.f, 0.f, 45.f));

		m_vLockChainRenders[1]->Set_LocalPosition(vector3(0.25f, 1.8f, 0.f));
		m_vLockChainRenders[1]->Set_LocalEulerAngles(vector3(0.f, 0.f, -45.f));
	}

	m_pLockRenderer->Get_GameObject()->SetActive(m_bLock);
}

void CDungeonGate::Update()
{
	__super::Update();

	if (m_bIsOpen)
	{
		if (!m_bLock)
			m_pBody->Set_LocalPositionY(Lerp(m_pBody->Get_LocalPosition().y, -4.f, DELTA_TIME));
		else
		{
			CTransform* lockTF = m_pLockRenderer->Get_Transform();
			const vector3 lerp = vector3::Lerp(lockTF->Get_LocalScale(), vector3::zero(), DELTA_TIME * 5.f);
			m_pLockRenderer->Get_Transform()->Set_LocalScale(lerp);

			if (lerp.x <= 0.1f)
				m_pBody->Set_LocalPositionY(Lerp(m_pBody->Get_LocalPosition().y, -4.f, DELTA_TIME));

			m_vLockChainRenders[0]->Set_LocalPosition(vector3::Lerp(m_vLockChainRenders[0]->Get_LocalPosition(), vector3(-1.5f, 3.f, 0.f), DELTA_TIME * 3.f));
			m_vLockChainRenders[1]->Set_LocalPosition(vector3::Lerp(m_vLockChainRenders[1]->Get_LocalPosition(), vector3(1.5f, 3.f, 0.f), DELTA_TIME * 3.f));
			m_vLockChainRenders[2]->Set_LocalPosition(vector3::Lerp(m_vLockChainRenders[2]->Get_LocalPosition(), vector3(0.f, -2.f, 0.f), DELTA_TIME * 3.f));
		}
	}
}

void CDungeonGate::OnTriggerStay(CCollider* _other)
{
	if (CInput::GetKeyDown(E))
	{
		if (m_bLock)
		{
			wstring item = L"DungeonKey";
			if (CGameManager::GetInstance().Get_Inventory()->UseItem(item))
				Open();
		}
	}

	if (CInput::GetKeyDown(T))
		Open();
}

void CDungeonGate::OnDestroy()
{
	__super::OnDestroy();
}

void CDungeonGate::SetLock()
{
	m_bLock = true;
}

void CDungeonGate::Open()
{
	m_bIsOpen = true;
}

void CDungeonGate::Close()
{
}
