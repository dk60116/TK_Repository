#include "cpch.h"
#include "Behaviour_Combat.h"

CBehaviour_Combat::CBehaviour_Combat()
	: m_fChangeRandomTime(0.f)
{
}

CBehaviour_Combat::~CBehaviour_Combat()
{
}

HRESULT CBehaviour_Combat::Initialize(CMonster* _monster)
{
	if (FAILED(__super::Initialize(_monster)))
		return E_FAIL;

	return S_OK;
}

void CBehaviour_Combat::Enter(void* _desc)
{
	__super::Enter();

	m_pMonster->Get_Animator()->SetLoop(false);
	m_pMonster->Get_Animator()->Play(L"Attack01", 0.1f);
	m_pMonster->PlaySoundEffect(L"Attack");
	m_pMonster->OnOffAttackTrigger(true);
}

void CBehaviour_Combat::During()
{
	__super::During();

	CTransform* playerTf = CGameManager::GetInstance().Get_Player()->Get_Transform();
	const vector3 playerPos = playerTf->Get_Position();
	const vector3 playerAngle = playerTf->Get_EulerAngles();

	CTransform* myTf = m_pMonster->Get_Transform();
	const vector3 myPos = myTf->Get_Position();
	const vector3 myAngle = myTf->Get_EulerAngles();

	if (m_pMonster->Get_Animator()->Get_StateInfo().normalizeTime >= 0.95f)
	{
		if (vector3::Distance(myPos, playerPos) <= m_pMonster->Get_Status().attackRange)
		{
			m_pMonster->OnOffAttackTrigger(false);
			m_pMonster->Change_State(CMonsterController::CombatWait);
		}
		else
		{
			m_pMonster->OnOffAttackTrigger(false);
			m_pMonster->Change_State(CMonsterController::Tracking);
		}

		m_fPassedTime = 0.f;
	}
}

void CBehaviour_Combat::Exit()
{
	__super::Exit();
}
