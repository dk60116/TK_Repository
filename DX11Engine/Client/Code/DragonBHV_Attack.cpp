#include "cpch.h"
#include "DragonBHV_Attack.h"
#include "Dragon.h"

CDragonBHV_Attack::CDragonBHV_Attack()
{
}

CDragonBHV_Attack::~CDragonBHV_Attack()
{
}

HRESULT CDragonBHV_Attack::Initialize(CMonster* _monster)
{
	m_iWeight = 2;

	if (FAILED(__super::Initialize(_monster)))
		return E_FAIL;

    return S_OK;
}

void CDragonBHV_Attack::Enter(void* _desc)
{
	__super::Enter(_desc);

	CDragon* dragon = dynamic_cast<CDragon*>(m_pMonster);
	
	dragon->Get_Animator()->Stop();
	dragon->PlayAttack_Ground();
	dragon->PlaySoundEffect(L"Attack");
}

void CDragonBHV_Attack::During()
{
	__super::During();

	CDragon* dragon = dynamic_cast<CDragon*>(m_pMonster);

	if (m_pMonster->Get_Animator()->Get_StateInfo().normalizeTime >= 0.95f)
		dragon->Get_Controller()->ChangeState(CBossController_Dragon::DCombatWait);
}

void CDragonBHV_Attack::Exit()
{
	__super::Exit();
}
