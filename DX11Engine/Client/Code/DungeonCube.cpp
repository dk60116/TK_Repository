#include "cpch.h"
#include "DungeonCube.h"

CDungeonCube::CDungeonCube()
	: m_pRigidBody(nullptr)
	, m_bSliding(false)
	, m_bPrevSliding(false)
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
	m_pGameObject->SetTag(L"DungeonCube");

	return S_OK;
}

void CDungeonCube::Awake()
{
	__super::Awake();

	m_vRenderer[0]->Get_Material()->Set_FloatValue(L"gSmoothness", 0.8f);

	if (!m_pRigidBody)
	{
		m_pRigidBody = m_pGameObject->AddComponent<CRigidBody>();
	}

	m_pAudioSource->SetLoop(true);
}

void CDungeonCube::Start()
{
	__super::Start();
}

void CDungeonCube::Update()
{
	__super::Update();

	vector3 playerPos = CGameManager::GetInstance().Get_Player()->Get_Transform()->Get_Position();
	vector3 myPos = Get_Transform()->Get_Position();

	m_pRigidBody->SetUseGravity(vector3::Distance(playerPos, myPos) <= 20.f);
}

void CDungeonCube::LateUpdate()
{
	m_bSliding = (abs(m_pRigidBody->Get_CollisionVector().x + m_pRigidBody->Get_CollisionVector().z)) > 0.f;

	if ((m_bSliding && !m_bPrevSliding) && m_pRigidBody->UseGravity())
		PlaySoundEffect(L"Rock");
	else if (!m_bSliding && m_bPrevSliding)
		StopSound();

	m_bPrevSliding = m_bSliding;
}

void CDungeonCube::OnTriggerEnter(CCollider* _other)
{
	
}

void CDungeonCube::OnDestroy()
{
}
