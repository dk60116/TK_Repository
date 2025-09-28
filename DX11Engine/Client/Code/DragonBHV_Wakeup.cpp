#include "cpch.h"
#include "DragonBHV_Wakeup.h"
#include "Dragon.h"

CDragonBHV_Wakeup::CDragonBHV_Wakeup()
	: m_bCompleteThreat(false)
	, m_bCompleteTakeFly(false)
{
}

CDragonBHV_Wakeup::~CDragonBHV_Wakeup()
{
}

HRESULT CDragonBHV_Wakeup::Initialize(CMonster* _monster)
{
	m_iWeight = 2;

	if (FAILED(__super::Initialize(_monster)))
		return E_FAIL;

    return S_OK;
}

void CDragonBHV_Wakeup::Enter(void* _desc)
{
	__super::Enter();

	CDragon* dragon = dynamic_cast<CDragon*>(m_pMonster);

	m_pMonster->Get_Animator()->Stop();
	dragon->PlayThreat();

	dragon->PlaySoundEffect(L"Threat");
}

void CDragonBHV_Wakeup::During()
{
	__super::During();

	CDragon* dragon = dynamic_cast<CDragon*>(m_pMonster);

	if (dragon->Get_Animator()->Get_StateInfo().normalizeTime >= 0.95f && !m_bCompleteThreat)
	{
		dragon->PlayGroundToFly();
		m_bCompleteThreat = true;
	}

	if (m_bCompleteThreat && m_fPassedTime >= 3.f)
	{
		if (dragon->Get_Animator()->Get_StateInfo().normalizeTime >= 0.95f && !m_bCompleteTakeFly)
		{
			dragon->SetFlying(true);
			CDragon::LandingDesc ld = { true, vector3(0.f, 10.f, -22.f)};
			dragon->Get_Controller()->ChangeState(CBossController_Dragon::Landing, &ld);
			m_bCompleteTakeFly = true;
		}
	}
}

void CDragonBHV_Wakeup::Exit()
{
	__super::Exit();
}
