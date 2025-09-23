#include "cpch.h"
#include "PlayerBehaviour_Bow.h"

CPlayerBehaviour_Bow::CPlayerBehaviour_Bow()
	: m_bAiming(false)
{
}

CPlayerBehaviour_Bow::~CPlayerBehaviour_Bow()
{
}

HRESULT CPlayerBehaviour_Bow::Initialize(CPlayer* _player)
{
	if (FAILED(__super::Initialize(_player)))
		return E_FAIL;

	m_iWeight = 2;

	return S_OK;
}

void CPlayerBehaviour_Bow::Enter(void* _desc)
{
	__super::Enter(_desc);

	m_pPlayer->PlayBowLoadAnimatoin();
	m_pPlayer->PopArrow();

	m_pPlayer->Get_Root()->Set_LocalEulerAnglesY(180.f);

	CGameManager::GetInstance().Get_PlayerCamera()->ChangeMode(CPlayerCamera::PlayerCamMode::BowAiming);
	CGameManager::GetInstance().Get_PlayerHUD()->OnOffBowCrossHair(true);

	m_bAiming = false;
}

void CPlayerBehaviour_Bow::During()
{
	__super::During();

	_float rotateY = Lerp(m_pPlayer->Get_Root()->Get_LocalEulerAngles().y, -90.f, DELTA_TIME * 6.f);
	m_pPlayer->Get_Root()->Set_LocalEulerAnglesY(rotateY);

	if (m_fPassedTime >= 1.f && !m_bAiming)
	{
		m_pPlayer->PlayBowAimingAnimation();
		m_bAiming = true;
	}

	const vector2 mouseDelta = vector2(CInput::GetAxis(L"Mosue MoveDelta X"), CInput::GetAxisRaw(L"Mosue MoveDelta Y"));
	CTransform* playerTF = m_pPlayer->Get_Transform();

	playerTF->Add_EulerAnglesY(mouseDelta.x * DELTA_TIME * m_pPlayer->Get_Status().aimingTurnSpeed);
	CGameManager::GetInstance().Get_PlayerCamera()->AddBowYValue(mouseDelta.y * DELTA_TIME * 3.f);

	if (CInput::GetMouseButtonDown(1))
	{
		m_pPlayer->ReturnArrow();
		m_pPlayer->Get_Controller()->ForceChangeState(CPlayerController::Move, &m_pPlayer->Get_Controller()->Get_MoveDesc());
	}

	if (CInput::GetMouseButtonUp(0))
	{
		if (m_bAiming)
		{
			if (CGameManager::GetInstance().Get_Inventory()->UseItem(L"Arrow"))
				m_pPlayer->ShootArrow();
		}
		else
			m_pPlayer->ReturnArrow();

		m_pPlayer->Get_Controller()->ForceChangeState(CPlayerController::Move, &m_pPlayer->Get_Controller()->Get_MoveDesc());
	}
}

void CPlayerBehaviour_Bow::Exit()
{
	__super::Exit();

	m_bAiming = false;

	CGameManager::GetInstance().Get_PlayerCamera()->ChangeMode(CPlayerCamera::PlayerCamMode::Default);
	CGameManager::GetInstance().Get_PlayerHUD()->OnOffBowCrossHair(false);
}
