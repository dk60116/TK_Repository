#include "cpch.h"
#include "Behaviour_GetHit.h"

CBehaviour_GetHit::CBehaviour_GetHit()
	: m_fChangeTime(1.f)
{
	m_bAnyTime = true;
}

CBehaviour_GetHit::~CBehaviour_GetHit()
{
}

HRESULT CBehaviour_GetHit::Initialize(CMonster* _monster)
{
	if (FAILED(__super::Initialize(_monster)))
		return E_FAIL;

	return S_OK;
}

void CBehaviour_GetHit::Enter()
{
	__super::Enter();

	m_pMonster->Get_Controller()->SetDamaged(true);
	m_pMonster->Get_Animator()->SetLoop(false);
	m_pMonster->Get_Animator()->Play(L"GetHit_Front", 0.1f);

	CDebug::Log("GetHit Enter");
}

void CBehaviour_GetHit::During()
{
	__super::During();

	if (m_fPassedTime >= m_fChangeTime)
	{
		m_pMonster->Change_State(CMonsterController::CombatWait);
	}
}

void CBehaviour_GetHit::Exit()
{
	__super::Exit();

	m_pMonster->Get_Controller()->SetDamaged(false);
}
