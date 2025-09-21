#include "cpch.h"
#include "Dragon.h"
#include "BossController_Dragon.h"
#include "ME_FireBall.h"

CDragon::CDragon()
	: m_sStatus({})
	, m_pController(nullptr)
	, m_bFlying(false)
	, m_pFireBallProto(nullptr)
	, m_pFireBreath(nullptr)
	, m_iShootFireCount(0)
{
}

CDragon::~CDragon()
{
}

CDragon* CDragon::Create()
{
	return new CDragon();
}

CComponent* CDragon::Clone() const
{
	CDragon* clone = new CDragon();

	return clone;
}

HRESULT CDragon::Initialize(void* _desc)
{
	m_strMonsterName = L"Dragon";

	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	m_pHeadTF = Get_Transform()->Find_ChildRecursive(L"MountainDragon_ Head");
	m_pBodyTF = Get_Transform()->Find_ChildRecursive(L"MountainDragon_ Pelvis");

	m_sOptions.headColliderCenter = vector3::right() * 0.5f;

	if (!m_pController)
	{
		m_pController = m_pGameObject->AddComponent<CBossController_Dragon>();
		m_pController->Set_Monster(this);
		__super::m_pController = m_pController;
	}

	if (m_pAnimator)
	{
		auto idle = Add_Animation(L"Idle");
		auto threat = Add_Animation(L"Threat");
		auto groundTofly = Add_Animation(L"GroundToFly");
		auto flyIdle = Add_Animation(L"FlyIdle");
		auto fly = Add_Animation(L"Fly");
		auto shootFireBall_fly = Add_Animation(L"ShootFireBall_Fly");
		auto getHit_fly = Add_Animation(L"GetHit_Fly");
		auto glide = Add_Animation(L"Glide");
		auto spreadFire_fly = Add_Animation(L"SpreadFire_Fly");
	}

	CGameObject* particleObj = m_pGameObject->Get_Scene()->Add_GameObject(L"PTC");
	m_pFireBreath = particleObj->AddComponent<CParticleSystem>();

	CGameObject* fireBallProtoObj = m_pGameObject->Get_Scene()->Add_GameObject(L"FireBall ProtoType");
	m_pFireBallProto = fireBallProtoObj->AddComponent<CME_FireBall>();

	return S_OK;
}

void CDragon::Awake()
{
	__super::Awake();
	
	CTransform* pelvTF = Get_Transform()->Find_ChildRecursive(L"MountainDragon_ Spine");
	CMonsterPartCollision* pelvCol = pelvTF->Get_GameObject()->AddComponent<CMonsterPartCollision>();
	pelvCol->Set_Monster(this);
	pelvCol->SetCenter(vector3(0.25f, 0.f, 0.f));
	pelvCol->SetSize(vector3(1.5f, 1.5f, 1.5f));
	m_mPartColList.emplace(L"Pelv", pelvCol);

	CTransform* chestTF = Get_Transform()->Find_ChildRecursive(L"MountainDragon_ Spine1");
	CMonsterPartCollision* chestCol = chestTF->Get_GameObject()->AddComponent<CMonsterPartCollision>();
	chestCol->Set_Monster(this);
	chestCol->SetCenter(vector3(0.25f, 0.f, 0.f));
	chestCol->SetSize(vector3(1.7f, 1.7f, 1.7f));
	m_mPartColList.emplace(L"Chest", chestCol);
}

void CDragon::Start()
{
	__super::Start();

	m_pFireBallProto->Get_Transform()->Set_Position(CGameManager::GetInstance().Get_Player()->Get_Transform()->Get_Position());
	//m_pFireBreath->Get_Transform()->Set_Position(CGameManager::GetInstance().Get_Player()->Get_Transform()->Get_Position() + vector3::up() * 2.f);
}

void CDragon::Update()
{
	__super::Update();
}

void CDragon::OnEnable()
{
	__super::OnEnable();
}

void CDragon::OnCollisionEnter(CCollider* _other)
{
	__super::OnCollisionEnter(_other);
}

void CDragon::OnDestroy()
{
	__super::OnDestroy();
}

CBossController_Dragon* CDragon::Get_Controller()
{
	return m_pController;
}

const CDragon::DragonStatus& CDragon::Get_Status()
{
	return m_sStatus;
}

void CDragon::Get_Damage(CWeapon* _weapon)
{
	_uint state = static_cast<_uint>(m_pController->Get_CrtState());
	m_pController->ChangeState(CBossController_Dragon::GetHit, &state);
}

void CDragon::PlayIdle(const _float _blending)
{
	if (!m_pAnimator)
		return;

	m_pAnimator->SetLoop(true);

	if (!m_bFlying)
	{
		m_pAnimator->Play(L"Idle", _blending);
	}
	else
	{
		m_pAnimator->Play(L"FlyIdle", _blending);
	}
}

void CDragon::PlayThreat(const _float _blending)
{
	if (!m_pAnimator)
		return;

	m_pAnimator->SetLoop(false);

	m_pAnimator->Play(L"Threat", _blending);
}

void CDragon::PlayGroundToFly(const _float _blending)
{
	if (!m_pAnimator)
		return;

	m_pAnimator->SetLoop(false);

	m_pAnimator->Play(L"GroundToFly", _blending);
}

void CDragon::PlayFly(const _float _blending)
{
	if (!m_pAnimator)
		return;

	m_pAnimator->SetLoop(true);

	m_pAnimator->Play(L"Fly", _blending);
}

void CDragon::PlayGetHit(const _float _blending)
{
	if (!m_pAnimator)
		return;

	m_pAnimator->SetLoop(false);

	if (!m_bFlying)
	{

	}
	else
	{
		m_pAnimator->Play(L"GetHit_Fly");
	}
}

void CDragon::PlayGlide(const _float _blending)
{
	if (!m_pAnimator)
		return;

	m_pAnimator->SetLoop(true);

	m_pAnimator->Play(L"Glide", _blending);
}

void CDragon::PlayShootFireball(const _float _blending)
{
	if (!m_pAnimator)
		return;

	m_pAnimator->SetLoop(false);

	m_pAnimator->Play(L"ShootFireBall_Fly", _blending);
}

void CDragon::PlaySpreadFire(const _float _blending)
{
	if (!m_pAnimator)
		return;

	m_pAnimator->SetLoop(false);

	if (!m_bFlying)
	{

	}
	else
	{
		m_pAnimator->Play(L"SpreadFire_Fly", _blending);
	}
}

void CDragon::ShootFireBall()
{
	m_pFireBallProto->Shoot(m_pHeadTF->Get_Position(), CGameManager::GetInstance().Get_Player()->Get_Transform()->Get_Position());

	++m_iShootFireCount;
}

const _uint CDragon::Get_ShootFireBallCount() const
{
	return m_iShootFireCount;
}

const _bool CDragon::GetFlying() const
{
	return m_bFlying;
}

void CDragon::SetFlying(const _bool _fly)
{
	m_bFlying = true;
}

const _uint CDragon::Get_ShootFireCount() const
{
	return m_iShootFireCount;
}
