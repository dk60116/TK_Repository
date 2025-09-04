#include "cpch.h"
#include "PlayerBehaviour_Ladder.h"
#include "Ladder.h"

CPlayerBehaviour_Ladder::CPlayerBehaviour_Ladder()
	: m_pLadder(nullptr)
	, m_iDirection(0)
	, m_iPrevDirection(0)
	, m_bOutUp(false)
	, m_fOutUpDuration(0.f)
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

	m_pPlayer->OnOffGravity(false);
	m_pPlayer->PlayLadderAnimation(0);
	m_pPlayer->Get_Transform()->Set_EulerAnglesY(m_pLadder->Get_Transform()->Get_EulerAngles().y + 270.f);

	m_bOutUp = false;
	m_fOutUpDuration = 0.f;
}

void CPlayerBehaviour_Ladder::During()
{
	__super::During();

	if (CInput::GetKey(W))
		m_iDirection = 1;
	if (CInput::GetKey(S))
		m_iDirection = -1;
	
	if (!CInput::GetKey(W) && !CInput::GetKey(S))
		m_iDirection = 0;

	if (m_iDirection != m_iPrevDirection)
	{
		m_pPlayer->PlayLadderAnimation(m_iDirection);
	}

	CTransform* playerTF = m_pPlayer->Get_Transform();

	if (!m_bOutUp)
		playerTF->Add_PositionY(3.f * static_cast<_float>(m_iDirection) * DELTA_TIME * 0.7f);

	m_iPrevDirection = m_iDirection;

	if (m_pLadder)
	{
		if (playerTF->Get_Position().y <= m_pLadder->Get_HeightValues().x)
		{
			if (CInput::GetKey(S))
				m_pPlayer->Get_Controller()->ForceChangeState(CPlayerController::Move, &m_pPlayer->Get_Controller()->Get_MoveDesc());
		}
		
		if (!m_pLadder)
			return;

		if (playerTF->Get_Position().y >= m_pLadder->Get_HeightValues().y && !m_bOutUp)
		{
			if (CInput::GetKey(W))
			{
				m_bOutUp = true;
				m_pPlayer->PlayLadderOutAnimation();
			}
		}

		if (m_bOutUp)
		{
			m_fOutUpDuration += DELTA_TIME;
			
			if (playerTF->Get_Position().y <= m_pLadder->Get_HeightValues().z + 0.25f)
				playerTF->Add_PositionY(DELTA_TIME * 2.f);
			else if (m_fOutUpDuration <= 1.f)
				playerTF->Add_Position(playerTF->Get_Directions().forward * DELTA_TIME * 1.5f);
			else
				m_pPlayer->Get_Controller()->ForceChangeState(CPlayerController::Move, &m_pPlayer->Get_Controller()->Get_MoveDesc());
		}
	}
}

void CPlayerBehaviour_Ladder::Exit()
{
	__super::Exit();

	m_pPlayer->OnOffGravity(true);

	m_pLadder = nullptr;

	m_bOutUp = false;
	m_fOutUpDuration = 0.f;
}
