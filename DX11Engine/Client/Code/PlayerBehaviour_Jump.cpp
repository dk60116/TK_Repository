#include "cpch.h"
#include "PlayerBehaviour_Jump.h"

CPlayerBehaviour_Jump::CPlayerBehaviour_Jump()
	: m_bDetached(false)
	, m_vDirection({})
{
}

CPlayerBehaviour_Jump::~CPlayerBehaviour_Jump()
{
}

HRESULT CPlayerBehaviour_Jump::Initialize(CPlayer* _player)
{
	if (FAILED(__super::Initialize(_player)))
		return E_FAIL;

	m_iWeight = 3;

	return S_OK;
}

void CPlayerBehaviour_Jump::Enter(void* _desc)
{
	__super::Enter();

	CPlayerController::MOVEDESC* desc = static_cast<CPlayerController::MOVEDESC*>(_desc);

	m_vDirection = desc->moveDirection;
	m_pPlayer->PlayJumpAnimation(m_vDirection);

	m_pPlayer->Get_Controller()->Set_MoveDirection(vector3::zero());
	m_pPlayer->Get_Controller()->Set_RotationDiretion(0.f);
	m_bDetached = false;

	if (m_vDirection.z > 0.f)
		m_pPlayer->Get_RigidBody()->AddForce(m_pPlayer->Get_Transform()->Get_Directions().forward * m_pPlayer->Get_Status().moveSpeed);
	else if (m_vDirection.z < 0.f)
		m_pPlayer->Get_RigidBody()->AddForce(m_pPlayer->Get_Transform()->Get_Directions().back * m_pPlayer->Get_Status().moveSpeed);
}

void CPlayerBehaviour_Jump::During()
{
	__super::During();

	if (m_fPassedTime >= 0.25f && !m_bDetached)
	{
		m_pPlayer->Get_RigidBody()->AddForce(vector3::up() * m_pPlayer->Get_Status().jumpForce);
		m_bDetached = true;
	}

	if (m_fPassedTime >= 1.f)
	{
		if (m_pPlayer->Get_GA().y < 0.1f)
		{
			m_pPlayer->Get_RigidBody()->SetVelocitY(0.f);
			m_pPlayer->Get_Controller()->ForceChangeState(CPlayerController::Move, &m_pPlayer->Get_Controller()->Get_MoveDesc());
		}
	}

	CAnimator* animator = m_pPlayer->Get_Animator();

	if (animator->Get_StateInfo().frame == 5)
	{
		if (animator->Get_StateInfo().startedFrame)
			m_pPlayer->PlaySoundEffect(L"Jump");
	}
}

void CPlayerBehaviour_Jump::Exit()
{
	__super::Exit();

	m_bDetached = false;
}
