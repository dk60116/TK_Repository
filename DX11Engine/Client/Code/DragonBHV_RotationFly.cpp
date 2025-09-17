#include "cpch.h"
#include "DragonBHV_RotationFly.h"
#include "Dragon.h"

CDragonBHV_RotationFly::CDragonBHV_RotationFly()
{
}

CDragonBHV_RotationFly::~CDragonBHV_RotationFly()
{
}

HRESULT CDragonBHV_RotationFly::Initialize(CMonster* _monster)
{
	m_iWeight = 4;

	if (FAILED(__super::Initialize(_monster)))
		E_FAIL;

	return S_OK;
}

void CDragonBHV_RotationFly::Enter(void* _desc)
{
	__super::Enter(_desc);

	CDragon* dragon = dynamic_cast<CDragon*>(m_pMonster);

	dragon->PlayGlide();
}

void CDragonBHV_RotationFly::During()
{
	__super::During();
}

void CDragonBHV_RotationFly::Exit()
{
	__super::Exit();
}
