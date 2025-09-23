#include "cpch.h"
#include "DungeonCube.h"

CDungeonCube::CDungeonCube()
	: m_pRigidBody(nullptr)
{
}

CDungeonCube::~CDungeonCube()
{
}

CDungeonCube* CDungeonCube::Create()
{
	return new CDungeonCube();
}

CComponent* CDungeonCube::Clone() const
{
	CDungeonCube* clone = new CDungeonCube();

	return clone;
}

HRESULT CDungeonCube::Initialize(void* _desc)
{
	m_strObjName = L"DungeonCube";

	m_sDescription.colliderCenter = vector3::up() * 0.75f;
	m_sDescription.colliderSize = vector3::one() * 1.5f;

	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	m_pGameObject->SetLayer(L"RigidObject");

	if (!m_pRigidBody)
	{
		m_pRigidBody = m_pGameObject->AddComponent<CRigidBody>();
		m_pRigidBody->SetKinematic(false);
		m_pRigidBody->SetUseGravity(true);
	}

	return S_OK;
}

void CDungeonCube::Awake()
{
	__super::Awake();
}

void CDungeonCube::Start()
{
	__super::Start();
}

void CDungeonCube::Update()
{
	__super::Update();
}

void CDungeonCube::OnTriggerEnter(CCollider* _other)
{
}

void CDungeonCube::OnDestroy()
{
}
