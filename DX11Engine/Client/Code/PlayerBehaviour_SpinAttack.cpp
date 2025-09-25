#include "cpch.h"
#include "PlayerBehaviour_SpinAttack.h"

CPlayerBehaviour_SpinAttack::CPlayerBehaviour_SpinAttack()
{
}

CPlayerBehaviour_SpinAttack::~CPlayerBehaviour_SpinAttack()
{
}

HRESULT CPlayerBehaviour_SpinAttack::Initialize(CPlayer* _player)
{
	m_iWeight = 2;

	if (FAILED(__super::Initialize(_player)))
		return E_FAIL;

    return S_OK;
}

void CPlayerBehaviour_SpinAttack::Enter(void* _desc)
{
	__super::Enter();

	m_pPlayer->PlaySpinAttackAnimation();
}

void CPlayerBehaviour_SpinAttack::During()
{
	__super::During();

	if (m_fPassedTime >= 1.5f)
	{
		_bool combatMode = true;
		m_pPlayer->Get_Controller()->ForceChangeState(CPlayerController::Idle, &combatMode);
	}

	CAnimator* animator = m_pPlayer->Get_Animator();

	if (animator->Get_StateInfo().frame == 5)
	{
		if (animator->Get_StateInfo().startedFrame)
			m_pPlayer->PlaySoundEffect(L"SwordAttack03");
	}
}

void CPlayerBehaviour_SpinAttack::Exit()
{
	__super::Exit();

	m_pPlayer->CloseSpinCollider();
}
