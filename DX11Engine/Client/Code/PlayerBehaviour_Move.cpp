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

	_float rotateY = Lerp(m_pPlayer->Get_Root()->Get_LocalEulerAngles().y, -180.f, DELTA_TIME * 6.f);
	m_pPlayer->Get_Root()->Set_LocalEulerAnglesY(rotateY);

	CTransform* playerTF = m_pPlayer->Get_Transform();
	const vector3& moveDir = m_pPlayer->Get_Controller()->Get_MoveDirection();
	const _float rotDir = m_pPlayer->Get_Controller()->Get_RotateDirection();
	_float speed = m_pPlayer->Get_Status().moveSpeed;

	if (moveDir.z < 0)
		speed *= m_pPlayer->Get_Status().backWalkRatio;

	if (moveDir != vector3::zero())
	{
		const vector3 dir = moveDir.normalized();
		playerTF->Add_Position(playerTF->Get_Directions().forward * dir.z * speed * DELTA_TIME);
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
