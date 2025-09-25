#include "cpch.h"
#include "DragonBHV_Dead.h"
#include "Dragon.h"

CDragonBHV_Dead::CDragonBHV_Dead()
{
}

CDragonBHV_Dead::~CDragonBHV_Dead()
{
}

HRESULT CDragonBHV_Dead::Initialize(CMonster* _monster)
{
	m_iWeight = 4;

	if (FAILED(__super::Initialize(_monster)))
		return E_FAIL;

	return S_OK;
}

void CDragonBHV_Dead::Enter(void* _desc)
{
	__super::Enter(_desc);

	CDragon* dragon = dynamic_cast<CDragon*>(m_pMonster);

	dragon->PlayDeath();
}

void CDragonBHV_Dead::During()
{
	__super::During();
}

void CDragonBHV_Dead::Exit()
{
	__super::Exit();
}
