#include "cpch.h"
#include "DragonBHV_Landing.h"
#include "Dragon.h"

CDragonBHV_Landing::CDragonBHV_Landing()
	: m_bThreadingRise(false)
	, m_vTargetPos({})
	, m_bCompleteRise(false)
{
}

CDragonBHV_Landing::~CDragonBHV_Landing()
{
}

HRESULT CDragonBHV_Landing::Initialize(CMonster* _monster)
{
	m_iWeight = 2;

	if (FAILED(__super::Initialize(_monster)))
		return E_FAIL;

	return S_OK;
}

void CDragonBHV_Landing::Enter(void* _desc)
{
	__super::Enter(_desc);

	if (_desc)
	{
		CDragon::LandingDesc desc = *static_cast<CDragon::LandingDesc*>(_desc);
		m_bThreadingRise = desc.threadRise;
		m_vTargetPos = desc.targetPos;
	}

	CDragon* dragon = dynamic_cast<CDragon*>(m_pMonster);

	dragon->SetFlying(true);

	if (m_bThreadingRise)
	{
		dragon->PlayIdle();
	}
	else
	{

	}
}

void CDragonBHV_Landing::During()
{
	__super::During();

	CDragon* dragon = dynamic_cast<CDragon*>(m_pMonster);

	CTransform* monsterTF = m_pMonster->Get_Transform();
	const vector3 monsterPos = monsterTF->Get_Position();

	if (m_bThreadingRise)
	{
		if (!m_bCompleteRise)
		{
			quaternion rotQ = monsterTF->LookQuaternion(m_vTargetPos, CTransform::X | CTransform::Z);

			monsterTF->Set_Quaternion(quaternion::Slerp(monsterTF->Get_Quaternion(), rotQ, DELTA_TIME * dragon->Get_Status().flyingTurnSpeed));

			if (monsterPos.y <= m_vTargetPos.y)
				monsterTF->Add_PositionY(dragon->Get_Status().riseSpeed * DELTA_TIME);
			else
			{
				dragon->PlayFly();
				m_bCompleteRise = true;
			}
		}
		else
		{
			const vector3 normal = (m_vTargetPos - monsterPos).normalized();

			if (vector3::Distance(monsterPos, m_vTargetPos) >= 0.05f)
				monsterTF->Add_Position(normal * 10.f * DELTA_TIME);
			else
				dragon->Get_Controller()->ChangeState(CBossController_Dragon::ShootFireBall);
		}
	}
}

void CDragonBHV_Landing::Exit()
{
	__super::Exit();
	
	m_bCompleteRise = false;
}
