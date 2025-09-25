#include "cpch.h"
#include "DragonBHV_RotationFly.h"
#include "Dragon.h"

CDragonBHV_RotationFly::CDragonBHV_RotationFly()
	: m_bGlide(false)
	, m_bFinal(false)
{
}

CDragonBHV_RotationFly::~CDragonBHV_RotationFly()
{
}

HRESULT CDragonBHV_RotationFly::Initialize(CMonster* _monster)
{
	m_iWeight = 2;

	if (FAILED(__super::Initialize(_monster)))
		E_FAIL;

	return S_OK;
}

void CDragonBHV_RotationFly::Enter(void* _desc)
{
	__super::Enter(_desc);

	CDragon* dragon = dynamic_cast<CDragon*>(m_pMonster);

	dragon->SetFlying(true);
	dragon->PlayIdle(0.1f);
}

void CDragonBHV_RotationFly::During()
{
	__super::During();

	CDragon* dragon = dynamic_cast<CDragon*>(m_pMonster);

	CTransform* myTF = m_pMonster->Get_Transform();

	if (m_fPassedTime <= 5.f)
	{
		myTF->Add_PositionY(dragon->Get_Status().riseSpeed * DELTA_TIME);
	}
	else if (m_fPassedTime <= 13.f)
	{
		if (!m_bGlide)
		{
			dragon->PlayGlide();
			m_bGlide = true;
		}

		myTF->Add_EulerAnglesY(45.f * DELTA_TIME);
		myTF->Add_Position(myTF->Get_Directions().forward * dragon->Get_Status().glideSpeed * DELTA_TIME);
	}
	else if (m_fPassedTime <= 21.f)
	{
		myTF->Add_EulerAnglesY(-45.f * DELTA_TIME);
		myTF->Add_Position(myTF->Get_Directions().forward * dragon->Get_Status().glideSpeed * DELTA_TIME);
	}
	else
	{
		if (!m_bFinal)
		{
			dragon->PlayIdle();
			m_bFinal = true;
		}

		if (m_fPassedTime <= 32.f)
			myTF->Add_PositionY(-dragon->Get_Status().riseSpeed * DELTA_TIME);
		else
			dragon->Get_Controller()->ChangeState(CBossController_Dragon::Grounding);
	}
}

void CDragonBHV_RotationFly::Exit()
{
	__super::Exit();
}
