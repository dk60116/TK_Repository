#include "cpch.h"
#include "PlayerBehaviour_Sword.h"
#include "Weapon.h"

CPlayerBehaviour_Sword::CPlayerBehaviour_Sword()
	: m_iCurrentCombo(0)
	, m_iPrevCombo(0)
	, m_bContinueCombo(false)
{
}

CPlayerBehaviour_Sword::~CPlayerBehaviour_Sword()
{
}

HRESULT CPlayerBehaviour_Sword::Initialize(CPlayer* _player)
{
	if (FAILED(__super::Initialize(_player)))
		return E_FAIL;

	m_iWeight = 2;

	return S_OK;
}

void CPlayerBehaviour_Sword::Enter(void* _desc)
{
	__super::Enter(_desc);

	m_iCurrentCombo = 0;
	m_iPrevCombo = 0;
	m_bContinueCombo = false;

	m_pPlayer->PlaySwordAnimation();

	m_pPlayer->Get_EqupWeapon()->OnOffCollider(true);
}

void CPlayerBehaviour_Sword::During()
{
	__super::During();

	CTransform* playerTF = m_pPlayer->Get_Transform();
	const vector3& moveDir = m_pPlayer->Get_Controller()->Get_MoveDirection();
	const _float rotDir = moveDir.z >= 0.f ? m_pPlayer->Get_Controller()->Get_RotateDirection() : -m_pPlayer->Get_Controller()->Get_RotateDirection();

	if (m_fPassedTime <= 1.f)
		m_iCurrentCombo = 0;
	else if (m_fPassedTime <= 1.7f)
		m_iCurrentCombo = 1;
	else if (m_fPassedTime <= 2.7f)
		m_iCurrentCombo = 2;
	else
		m_pPlayer->Get_Controller()->ForceChangeState(CPlayerController::Move, &m_pPlayer->Get_Controller()->Get_MoveDesc());

	if (m_fPassedTime >= 0.25f && CInput::GetMouseButtonDown(0))
		m_bContinueCombo = true;

	if (m_iCurrentCombo != m_iPrevCombo)
	{
		if (!m_bContinueCombo)
		{
			if (moveDir.z != 0)
				m_pPlayer->Get_Controller()->ForceChangeState(CPlayerController::Move, &m_pPlayer->Get_Controller()->Get_MoveDesc());
			else
			{
				_bool combatMode = true;
				m_pPlayer->Get_Controller()->ForceChangeState(CPlayerController::Idle, &combatMode);
			}
		}
		else
		{
			m_pPlayer->Get_EqupWeapon()->OnOffCollider(true);
		}
		
		m_bContinueCombo = false;
	}

	if (moveDir != vector3::zero())
	{
		const vector3 dir = moveDir.normalized();
		playerTF->Add_Position(playerTF->Get_Directions().forward * dir.z * m_pPlayer->Get_Status().moveSpeed * m_pPlayer->Get_Status().attackWalkRatio * DELTA_TIME);
	}

	if (rotDir != 0.f)
	{
		if (rotDir != 0.f)
			playerTF->Add_EulerAnglesY(rotDir * m_pPlayer->Get_Status().turnSpeed * DELTA_TIME);
	}

	m_iPrevCombo = m_iCurrentCombo;
}

void CPlayerBehaviour_Sword::Exit()
{
	__super::Exit();

	m_pPlayer->Get_EqupWeapon()->OnOffCollider(false);

	m_iCurrentCombo = 0;
	m_iPrevCombo = 0;

	m_bContinueCombo = false;
}
