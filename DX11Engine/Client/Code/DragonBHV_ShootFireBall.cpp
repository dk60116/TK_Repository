#include "cpch.h"
#include "DragonBHV_ShootFireBall.h"
#include "Dragon.h"

CDragonBHV_ShootFireBall::CDragonBHV_ShootFireBall()
	: m_bShooted(false)
{
}

CDragonBHV_ShootFireBall::~CDragonBHV_ShootFireBall()
{
}

HRESULT CDragonBHV_ShootFireBall::Initialize(CMonster* _monster)
{
	m_iWeight = 2;

	if (FAILED(__super::Initialize(_monster)))
		E_FAIL;

	return S_OK;
}

void CDragonBHV_ShootFireBall::Enter(void* _desc)
{
	__super::Enter();

	m_pMonster->Get_Animator()->SetLoop(true);
	m_pMonster->Get_Animator()->Play(L"FlyIdle", 0.3f);
}

void CDragonBHV_ShootFireBall::During()
{
	__super::During();

	CDragon* dragon = dynamic_cast<CDragon*>(m_pMonster);

	CTransform* playerTf = CGameManager::GetInstance().Get_Player()->Get_Transform();
	const vector3 playerPos = playerTf->Get_Position();
	const vector3 playerAngle = playerTf->Get_EulerAngles();

	CTransform* myTf = m_pMonster->Get_Transform();
	const vector3 myPos = myTf->Get_Position() + vector3::up() * 0.5f;
	const vector3 myAngle = myTf->Get_EulerAngles();

	quaternion rotQ = myTf->LookQuaternion(playerPos, CTransform::X | CTransform::Z);

	myTf->Set_Quaternion(quaternion::Slerp(myTf->Get_Quaternion(), rotQ, DELTA_TIME * dragon->Get_Status().flyingTurnSpeed));

	if (m_fPassedTime >= dragon->Get_Status().flyingTurnSpeed && !m_bShooted)
	{
		if (dragon->Get_ShootFireCount() < 4)
		{
			dragon->PlayShootFireball();
			dragon->PlaySoundEffect(L"Shooting");

			m_bShooted = true;
			m_fPassedTime = 0.f;
		}
		else
			dragon->Get_Controller()->ChangeState(CBossController_Dragon::RotateFly);
	}

	CAnimator* animator = dragon->Get_Animator();

	if (m_bShooted)
	{
		if (animator->Get_StateInfo().frame == 10)
		{
			if (animator->Get_StateInfo().startedFrame)
				dragon->ShootFireBall();
		}

		if (m_fPassedTime >= 0.7f)
		{
			dragon->PlayIdle();
			m_bShooted = false;
		}
	}
	else
	{
		if (animator->Get_StateInfo().frame == 5)
		{
			if (animator->Get_StateInfo().startedFrame)
				m_pMonster->PlaySoundEffect(L"Wing");
		}
	}
}

void CDragonBHV_ShootFireBall::Exit()
{
	__super::Exit();
}
