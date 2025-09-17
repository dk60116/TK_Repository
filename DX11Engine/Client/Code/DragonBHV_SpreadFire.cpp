#include "cpch.h"
#include "DragonBHV_SpreadFire.h"
#include "Dragon.h"

CDragonBHV_SpreadFire::CDragonBHV_SpreadFire()
{
}

CDragonBHV_SpreadFire::~CDragonBHV_SpreadFire()
{
}

HRESULT CDragonBHV_SpreadFire::Initialize(CMonster* _monster)
{
	m_iWeight = 2;

	if (FAILED(__super::Initialize(_monster)))
		return E_FAIL;

	return S_OK;
}

void CDragonBHV_SpreadFire::Enter(void* _desc)
{
	__super::Enter(_desc);

	CDragon* dragon = dynamic_cast<CDragon*>(m_pMonster);

	dragon->PlaySpreadFire();
}

void CDragonBHV_SpreadFire::During()
{
	__super::During();
}

void CDragonBHV_SpreadFire::Exit()
{
	__super::Exit();
}
