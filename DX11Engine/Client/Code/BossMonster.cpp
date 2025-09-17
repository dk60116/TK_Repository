#include "cpch.h"
#include "BossMonster.h"

CBossMonster::CBossMonster()
	: m_sDesc({})
{
}

CBossMonster::~CBossMonster()
{
}

HRESULT CBossMonster::Initialize(void* _desc)
{
	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	return S_OK;
}

void CBossMonster::Awake()
{
	__super::Awake();
}

void CBossMonster::Start()
{
	__super::Start();

	m_pRigidBody->SetUseGravity(false);
}

void CBossMonster::Update()
{
	__super::Update();
}

void CBossMonster::OnEnable()
{
	__super::OnEnable();
}

void CBossMonster::OnCollisionEnter(CCollider* _other)
{
}

void CBossMonster::OnDestroy()
{
	__super::OnDestroy();
}

const wstring& CBossMonster::Get_MonsterName()
{
	return m_strName;
}

CAnimator* CBossMonster::Get_Animator() const
{
	return m_pAnimator;
}

void CBossMonster::Get_Damage(CWeapon* _weapon)
{
}
