#include "cpch.h"
#include "Dragon.h"
#include "BossController_Dragon.h"

CDragon::CDragon()
	:m_pController(nullptr)
	, m_bFlying(false)
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
	}

	return S_OK;
}

void CDragon::Awake()
{
	__super::Awake();
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

const _bool CDragon::GetFlying() const
{
	return m_bFlying;
}

void CDragon::SetFlying(const _bool _fly)
{
	m_bFlying = true;
}
