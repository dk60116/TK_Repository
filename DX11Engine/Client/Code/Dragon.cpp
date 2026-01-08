#include "cpch.h"
#include "Dragon.h"
#include "BossController_Dragon.h"
#include "ME_FireBall.h"
#include "Weapon.h"

CDragon::CDragon()
	: m_sStatus({})
	, m_pController(nullptr)
	, m_bFlying(false)
	, m_pFireBallProto(nullptr)
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

	m_sOptions.colliderCenter = vector3::up() * 1.5f;
	m_sOptions.colliderSize = vector3(2.f, 3.f, 3.f);

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
		auto groundToFly = Add_Animation(L"GroundToFly");
		auto flyIdle = Add_Animation(L"FlyIdle");
		auto fly = Add_Animation(L"Fly");
		auto shootFireBall_Fly = Add_Animation(L"ShootFireBall_Fly");
		auto getHit_fly = Add_Animation(L"GetHit_Fly");
		auto getHit_Ground = Add_Animation(L"GetHit_Ground");
		auto glide = Add_Animation(L"Glide");
		auto spreadFire_Fly = Add_Animation(L"SpreadFire_Fly");
		auto grounding = Add_Animation(L"Grounding");
		auto run = Add_Animation(L"Run");
		auto attack01G = Add_Animation(L"Attack01_Ground");
		auto walkTurnL = Add_Animation(L"WalkTurn_Left");
		auto walkTurnR = Add_Animation(L"WalkTurn_Right");
		auto death_Ground = Add_Animation(L"Death_Ground");
	}

	if (m_pAudioSource)
	{
		Add_Sound(L"Threat");
		Add_Sound(L"Wing");
		Add_Sound(L"Shooting");
		Add_Sound(L"GetHit");
		Add_Sound(L"Attack");
		Add_Sound(L"Dead");
	}

	CGameObject* fireBallProtoObj = m_pGameObject->Get_Scene()->Add_GameObject(L"FireBall ProtoType");
	m_pFireBallProto = fireBallProtoObj->AddComponent<CME_FireBall>();
	m_pFireBallProto->Get_GameObject()->SetActive(false);

	m_vMeshRenderers[0]->Get_Material()->Set_Texture(CResources::LoadOnScene<CTexture>(L"Dragon_NormalMap (Texture)"), 1);
	m_vMeshRenderers[0]->Get_Material()->Set_FloatValue(L"gSmoothness", 0.5f);

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
	//m_pFireBallProto->Get_Transform()->Set_Position(CGameManager::GetInstance().Get_Player()->Get_Transform()->Get_Position() + vector3::up() * 2.f);
	//m_pFireBallProto->Get_GameObject()->SetActive(true);
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
	m_pController->ChangeState(CBossController_Dragon::DGetHit, &state);

	m_sStatus.crtHp -= _weapon->Get_Stat().attack;

	if (m_sStatus.crtHp <= 0)
		Death();
}

void CDragon::Death()
{
	__super::Death();
	m_pController->ChangeState(CBossController_Dragon::DDeath);
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
		m_pAnimator->Play(L"GetHit_Ground", _blending);
	}
	else
	{
		m_pAnimator->Play(L"GetHit_Fly", _blending);
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

void CDragon::PlaySpreadGrounding(const _float _blending)
{
	if (!m_pAnimator)
		return;

	m_pAnimator->SetLoop(false);

	m_pAnimator->Play(L"Grounding", _blending);
}

void CDragon::PlayRun(const _float _blending)
{
	m_pAnimator->SetLoop(true);

	m_pAnimator->Play(L"Run", _blending);
}

void CDragon::PlayAttack_Ground(const _float _blending)
{
	m_pAnimator->SetLoop(false);

	m_pAnimator->Play(L"Attack01_Ground", _blending);
}

void CDragon::PlayDeath(const _float _blending)
{
	m_pAnimator->SetLoop(false);

	m_pAnimator->Play(L"Death_Ground", _blending);
}

void CDragon::ShootFireBall()
{
	m_pFireBallProto->Get_GameObject()->SetActive(true);
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
	m_bFlying = _fly;

	m_pRigidBody->SetUseGravity(!m_bFlying);
}

const _uint CDragon::Get_ShootFireCount() const
{
	return m_iShootFireCount;
}
