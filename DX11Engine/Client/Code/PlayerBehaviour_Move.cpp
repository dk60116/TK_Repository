#include "cpch.h"
#include "PlayerBehaviour_Move.h"

CPlayerBehaviour_Move::CPlayerBehaviour_Move()
{
}

CPlayerBehaviour_Move::~CPlayerBehaviour_Move()
{
}

HRESULT CPlayerBehaviour_Move::Initialize(CPlayer* _player)
{
	if (FAILED(__super::Initialize(_player)))
		return E_FAIL;

	m_iWeight = 1;

	return S_OK;
}

void CPlayerBehaviour_Move::Enter(void* _desc)
{
	__super::Enter(_desc);

	CPlayerController::MOVEDESC* desc = static_cast<CPlayerController::MOVEDESC*>(_desc);
	m_pPlayer->PlayMoveAnimation(desc->moveDirection, desc->rotateDirection);
}

void CPlayerBehaviour_Move::During()
{
	__super::During();

	const vector3& moveDir = m_pPlayer->Get_Controller()->Get_MoveDirection();
	const _float rotDir = moveDir.z >= 0.f ? m_pPlayer->Get_Controller()->Get_RotateDirection() : -m_pPlayer->Get_Controller()->Get_RotateDirection();
	CTransform* playerTF = m_pPlayer->Get_Transform();

	if (moveDir != vector3::zero())
	{
		const vector3 dir = moveDir.normalized();
		playerTF->Add_Position(playerTF->Get_Directions().forward * dir.z * m_pPlayer->Get_Status().moveSpeed * DELTA_TIME);
	}

	if (rotDir != 0.f)
	{
		if (rotDir != 0.f)
			playerTF->Add_EulerAnglesY(rotDir * m_pPlayer->Get_Status().turnSpeed * DELTA_TIME);
	}

	if (moveDir == vector3::zero() && rotDir == 0)
		m_pPlayer->Get_Controller()->ChangeState(CPlayerController::Idle);
}

void CPlayerBehaviour_Move::Exit()
{
	__super::Exit();
}
