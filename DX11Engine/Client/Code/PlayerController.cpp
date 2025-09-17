#include "cpch.h"
#include "PlayerController.h"
#include "Weapon.h"
#include "PlayerBehaviour_Idle.h"
#include "PlayerBehaviour_Move.h"
#include "PlayerBehaviour_Sword.h"
#include "PlayerBehaviour_Bow.h"
#include "PlayerBehaviour_Jump.h"
#include "PlayerBehaviour_Ladder.h"

CPlayerController::CPlayerController()
	: m_pPlayer(nullptr)
	, m_pFocusTransform(nullptr)
	, m_eCrtState(PlayerState::None)
	, m_mBehaviourList({})
	, m_pCrtBehaviour(nullptr)
	, m_bDamaged(false)
	, m_bDead(false)
	, m_sMoveDesc({})
{
}

CPlayerController::~CPlayerController()
{
}

CPlayerController* CPlayerController::Create()
{
	return new CPlayerController();
}

CComponent* CPlayerController::Clone() const
{
	return new CPlayerController();
}

HRESULT CPlayerController::Initialize(void* _desc)
{
	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	return S_OK;
}

void CPlayerController::Awake()
{
	AddBehaviour<CPlayerBehaviour_Idle>(Idle);
	AddBehaviour<CPlayerBehaviour_Move>(Move);
	AddBehaviour<CPlayerBehaviour_Sword>(Sword);
	AddBehaviour<CPlayerBehaviour_Jump>(Jump);
	AddBehaviour<CPlayerBehaviour_Bow>(Bow);
	AddBehaviour<CPlayerBehaviour_Ladder>(Ladder);

	ChangeState(Idle);
}

void CPlayerController::Start()
{
}

void CPlayerController::Update()
{
	UpdateControleState();

	if (m_pCrtBehaviour)
		m_pCrtBehaviour->During();
}

void CPlayerController::OnDestroy()
{
	Safe_Release(m_pPlayer);

	for (TRAVERSAL_ITER(m_mBehaviourList, it))
		Safe_Release((*it).second);

	m_mBehaviourList.clear();
}

CPlayer* CPlayerController::Get_Player()
{
	return m_pPlayer;
}

void CPlayerController::Set_Player(CPlayer* _player)
{
	if (_player == m_pPlayer)
		return;

	m_pPlayer = _player;

	if (m_pPlayer)
		m_pPlayer->AddRef();
}

const CPlayerController::PlayerState CPlayerController::Get_State() const
{
	return m_eCrtState;
}

void CPlayerController::Set_Focus(CTransform* _target)
{
	m_pFocusTransform = _target;
}

void CPlayerController::ChangeState(const PlayerState _state, const _bool _forceEnter, void* _enterDesc)
{
	if (!_forceEnter)
	{
		if (_state == m_eCrtState)
			return;
	}

	if (m_pCrtBehaviour && m_pCrtBehaviour->m_iWeight > m_mBehaviourList[_state]->m_iWeight)
		return;

	if (m_pCrtBehaviour)
		m_pCrtBehaviour->Exit();

	m_pCrtBehaviour = m_mBehaviourList[_state];
	m_pCrtBehaviour->Enter(_enterDesc);

	m_eCrtState = _state;
}

void CPlayerController::ForceChangeState(const PlayerState _state, void* _enterDesc)
{
	if (m_pCrtBehaviour)
		m_pCrtBehaviour->Exit();

	m_pCrtBehaviour = m_mBehaviourList[_state];
	m_pCrtBehaviour->Enter(_enterDesc);

	m_eCrtState = _state;
}

const vector3& CPlayerController::Get_MoveDirection()
{
	return m_sMoveDesc.moveDirection;
}

void CPlayerController::Set_MoveDirection(const vector3& _value)
{
	m_sMoveDesc.moveDirection = _value;
}

const float CPlayerController::Get_RotateDirection()
{
	return m_sMoveDesc.rotateDirection;
}

void CPlayerController::Set_RotationDiretion(const _float _value)
{
	m_sMoveDesc.rotateDirection = _value;
}

CPlayerController::MOVEDESC& CPlayerController::Get_MoveDesc()
{
	return m_sMoveDesc;
}

void CPlayerController::UpdateControleState()
{
	if (CInput::GetKeyDown(SPACE))
		ChangeState(Jump, false, &m_sMoveDesc);

	if (CInput::GetMouseButtonDown(0))
	{
		if (m_pPlayer->Get_EqupWeapon())
		{
			switch (m_pPlayer->Get_EqupWeapon()->Get_WeaponType())
			{
			case CGameManager::WeaponType::Sword:
				ChangeState(Sword);
				break;
			case CGameManager::WeaponType::Bow:
				ChangeState(Bow);
				break;
			}
		}
	}

	m_sMoveDesc.moveDirection = vector3::zero();
	m_sMoveDesc.rotateDirection = 0.f;

	if (CInput::GetKey(W))
		m_sMoveDesc.moveDirection += vector3::forward();
	if (CInput::GetKey(S))
		m_sMoveDesc.moveDirection += vector3::back();

	if (CInput::GetKey(A))
		m_sMoveDesc.rotateDirection -= 1.f;
	if (CInput::GetKey(D))
		m_sMoveDesc.rotateDirection += 1.f;

	if (m_sMoveDesc.moveDirection != m_sMoveDesc.prevMoveDir)
		ChangeState(Move, true, &m_sMoveDesc);

	if (m_sMoveDesc.moveDirection.z == 0 && m_sMoveDesc.rotateDirection != m_sMoveDesc.prevRotateDir)
		ChangeState(Move, true, &m_sMoveDesc);

	m_sMoveDesc.prevMoveDir = m_sMoveDesc.moveDirection;
	m_sMoveDesc.prevRotateDir = m_sMoveDesc.rotateDirection;
}
