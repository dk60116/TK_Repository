#include "cpch.h"
#include "Behaviour_Tracking.h"

CBehaviour_Tracking::CBehaviour_Tracking()
{
}

CBehaviour_Tracking::~CBehaviour_Tracking()
{
}

HRESULT CBehaviour_Tracking::Initialize(CMonster* _monster)
{
	if (FAILED(__super::Initialize(_monster)))
		E_FAIL;

	return S_OK;
}

void CBehaviour_Tracking::Enter()
{
	__super::Enter();

	m_pMonster->Get_Animator()->SetLoop(true);
	m_pMonster->Get_Animator()->Play(L"Run", 0.1f);
}

void CBehaviour_Tracking::During()
{
	__super::During();

	CTransform* playerTf = CGameManager::GetInstance().Get_Player()->Get_Transform();
	const vector3 playerPos = playerTf->Get_Position();
	const vector3 playerAngle = playerTf->Get_EulerAngles();

	CTransform* myTf = m_pMonster->Get_Transform();
	const vector3 myPos = myTf->Get_Position();
	const vector3 myAngle = myTf->Get_EulerAngles();

	if (vector3::Distance(myPos, playerPos) < m_pMonster->Get_Status().attackRange)
	{
		m_pMonster->Change_State(CMonsterController::Combat);
	}
}

void CBehaviour_Tracking::Exit()
{
	__super::Exit();
}
