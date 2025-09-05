#include "cpch.h"
#include "DragonBHV_Idle.h"
#include "Dragon.h"

CDragonBHV_Idle::CDragonBHV_Idle()
{
}

CDragonBHV_Idle::~CDragonBHV_Idle()
{
}

HRESULT CDragonBHV_Idle::Initialize(class CBossMonster* _boss)
{
	m_iWeight = 1;

	if (FAILED(__super::Initialize(_boss)))
		return E_FAIL;

	return S_OK;
}

void CDragonBHV_Idle::Enter(void* _desc)
{
	__super::Enter(_desc);
	
	m_pBoss->PlayIdle(0.3f);
}

void CDragonBHV_Idle::During()
{
	__super::During();

	CDragon* dragon = dynamic_cast<CDragon*>(m_pBoss);

	if (!dragon->GetFlying() && m_fPassedTime >= 5.f)
		dragon->Get_Controller()->Change_State(1);
}

void CDragonBHV_Idle::Exit()
{
	__super::Exit();
}
