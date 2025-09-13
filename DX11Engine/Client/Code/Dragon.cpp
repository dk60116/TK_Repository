#include "cpch.h"
#include "Dragon.h"
#include "BossController_Dragon.h"
#include "ME_FireBall.h"

CDragon::CDragon()
	: m_sStatus({})
	, m_pController(nullptr)
	, m_bFlying(false)
	, m_pFireBallProto(nullptr)
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

HRESULT CDragon::Initialize()
{
	m_strBossName = L"Dragon";

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (!m_pController)
	{
		m_pController = m_pGameObject->AddComponent<CBossController_Dragon>();
		m_pController->Set_Monster(this);
	}

	if (m_pAnimator)
	{
		Add_Animation(L"Idle");
		Add_Animation(L"Threat");
		Add_Animation(L"GroundToFly");
		Add_Animation(L"FlyIdle");
		Add_Animation(L"Fly");
		Add_Animation(L"ShootFireBall_Fly");
	}

	CGameObject* fireBallProtoObj = m_pGameObject->Get_Scene()->Add_GameObject(L"FireBall ProtoType");
	m_pFireBallProto = fireBallProtoObj->AddComponent<CME_FireBall>();

	return S_OK;
}

void CDragon::Awake()
{
	__super::Awake();

	m_pFireBallProto->Get_Transform()->Set_Position(CGameManager::GetInstance().Get_Player()->Get_Transform()->Get_Position());
}

void CDragon::Start()
{
	__super::Start();
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

void CDragon::PlayShootFireball(const _float _blending)
{
	if (!m_pAnimator)
		return;

	m_pAnimator->SetLoop(false);

	m_pAnimator->Play(L"ShootFireBall_Fly", _blending);
}

void CDragon::ShootFireBall()
{
}

const _bool CDragon::GetFlying() const
{
	return m_bFlying;
}

void CDragon::SetFlying(const _bool _fly)
{
	m_bFlying = true;
}
