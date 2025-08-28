#include "cpch.h"
#include "PlayerBehaviour.h"

CPlayerBehaviour::CPlayerBehaviour()
	: m_iWeight(9999)
	, m_pPlayer(nullptr)
	, m_strBehaviourName(L"")
	, m_fPassedTime(0.f)
	, m_bAnyTime(false)
{
}

CPlayerBehaviour::~CPlayerBehaviour()
{
}

HRESULT CPlayerBehaviour::Initialize(CPlayer* _player)
{
	m_pPlayer = _player;

	if (!m_pPlayer)
		return E_FAIL;

	return S_OK;
}

void CPlayerBehaviour::Enter(void* _desc)
{
	m_fPassedTime = 0.f;

	m_pPlayer->Get_Animator()->Stop();
}

void CPlayerBehaviour::During()
{
	m_fPassedTime += DELTA_TIME;
}

void CPlayerBehaviour::Exit()
{
	m_fPassedTime = 0.f;
}

void CPlayerBehaviour::OnDestroy()
{
}
