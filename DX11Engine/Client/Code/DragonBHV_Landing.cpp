#include "cpch.h"
#include "DragonBHV_Landing.h"
#include "Dragon.h"

CDragonBHV_Landing::CDragonBHV_Landing()
	: m_bThreadingRise(false)
	, m_vTargetPos({})
{
}

CDragonBHV_Landing::~CDragonBHV_Landing()
{
}

HRESULT CDragonBHV_Landing::Initialize(CBossMonster* _boss)
{
	m_iWeight = 1;

	if (FAILED(__super::Initialize(_boss)))
		return E_FAIL;

	return S_OK;
}

void CDragonBHV_Landing::Enter(void* _desc)
{
	__super::Enter(_desc);

	if (_desc)
	{
		CDragon::LandingDesc desc = *static_cast<CDragon::LandingDesc*>(_desc);
		m_bThreadingRise = desc.threadRise;
		m_vTargetPos = desc.targetPos;
	}

	CDragon* dragon = dynamic_cast<CDragon*>(m_pBoss);

	dragon->SetFlying(true);

	if (m_bThreadingRise)
	{
		m_pBoss->PlayIdle();
	}
	else
	{

	}
}

void CDragonBHV_Landing::During()
{
	__super::During();

	CTransform* monsterTF = m_pBoss->Get_Transform();
	const vector3 monsterPos = monsterTF->Get_Position();

	if (m_bThreadingRise)
	{
		if (monsterPos.y <= m_vTargetPos.y)
			monsterTF->Add_PositionY(DELTA_TIME);
	}
}

void CDragonBHV_Landing::Exit()
{
	__super::Exit();
}
