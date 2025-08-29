#include "cpch.h"
#include "PlayerBehaviour_Idle.h"

CPlayerBehaviour_Idle::CPlayerBehaviour_Idle()
{
}

CPlayerBehaviour_Idle::~CPlayerBehaviour_Idle()
{
}

HRESULT CPlayerBehaviour_Idle::Initialize(CPlayer* _player)
{
	if (FAILED(__super::Initialize(_player)))
		return E_FAIL;

	m_iWeight = 1;

	return S_OK;
}

void CPlayerBehaviour_Idle::Enter(void* _desc)
{
	__super::Enter();

	_bool combat = false;

	if (_desc)
		combat = *static_cast<_bool*>(_desc);

	m_bCombatMode = combat;

	m_pPlayer->PlayIdleAnimation(m_bCombatMode);
}

void CPlayerBehaviour_Idle::During()
{
	__super::During();

	_float rotateY = Lerp(m_pPlayer->Get_Root()->Get_LocalEulerAngles().y, -180.f, DELTA_TIME * 6.f);
	m_pPlayer->Get_Root()->Set_LocalEulerAnglesY(rotateY);

	if (m_bCombatMode)
	{
		if (m_fPassedTime >= 5.f)
			m_pPlayer->Get_Controller()->ForceChangeState(CPlayerController::Idle);
	}
}

void CPlayerBehaviour_Idle::Exit()
{
	__super::Exit();
}
