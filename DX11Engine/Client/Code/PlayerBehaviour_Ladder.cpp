#include "cpch.h"
#include "PlayerBehaviour_Ladder.h"
#include "Ladder.h"

CPlayerBehaviour_Ladder::CPlayerBehaviour_Ladder()
	: m_pLadder(nullptr)
	, m_iDirection(0)
	, m_iPrevDirection(0)
{
}

CPlayerBehaviour_Ladder::~CPlayerBehaviour_Ladder()
{
}

HRESULT CPlayerBehaviour_Ladder::Initialize(CPlayer* _player)
{
	if (FAILED(__super::Initialize(_player)))
		return  S_OK;

	m_iWeight = 3;

	return S_OK;
}

void CPlayerBehaviour_Ladder::Enter(void* _desc)
{
	__super::Enter();

	if (_desc)
		m_pLadder = static_cast<CLadder*>(_desc);

	m_pPlayer->PlayLadderAnimation(0);
	m_pPlayer->Get_Transform()->Set_EulerAnglesY(m_pLadder->Get_Transform()->Get_EulerAngles().y + 270.f);
}

void CPlayerBehaviour_Ladder::During()
{
	__super::During();

	if (CInput::GetKey(W))
		m_iDirection = 1;
	if (CInput::GetKey(S))
		m_iDirection = -1;
	else
		m_iDirection = 0;

	if (m_iDirection != m_iPrevDirection)
	{
		m_pPlayer->PlayLadderAnimation(m_iDirection);
	}

	m_iPrevDirection = m_iDirection;

	CTransform* playerTF = m_pPlayer->Get_Transform();

	playerTF->Add_PositionY(3.f * static_cast<_float>(m_iDirection) * DELTA_TIME);

}

void CPlayerBehaviour_Ladder::Exit()
{
	__super::Exit();
}
