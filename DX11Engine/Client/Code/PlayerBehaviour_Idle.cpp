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

	m_pPlayer->PlayIdleAnimation();
}

void CPlayerBehaviour_Idle::During()
{
	__super::During();
}

void CPlayerBehaviour_Idle::Exit()
{
	__super::Exit();
}
