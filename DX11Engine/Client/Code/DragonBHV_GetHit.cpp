#include "cpch.h"
#include "DragonBHV_GetHit.h"
#include "Dragon.h"

CDragonBHV_GetHit::CDragonBHV_GetHit()
	: m_iPrevState(0)
{
}

CDragonBHV_GetHit::~CDragonBHV_GetHit()
{
}

HRESULT CDragonBHV_GetHit::Initialize(CMonster* _monster)
{
	m_iWeight = 3;

	if (FAILED(__super::Initialize(_monster)))
		return E_FAIL;

	return S_OK;
}

void CDragonBHV_GetHit::Enter(void* _desc)
{
	__super::Enter(_desc);

	if (_desc)
		m_iPrevState = *static_cast<_uint*>(_desc);

	CDragon* dragon = dynamic_cast<CDragon*>(m_pMonster);

	dragon->PlayGetHit();
}

void CDragonBHV_GetHit::During()
{
	CDragon* dragon = dynamic_cast<CDragon*>(m_pMonster);

	if (dragon->Get_Animator()->Get_StateInfo().normalizeTime >= 0.95f)
	{
		dragon->Get_Controller()->ForceChangeState(m_iPrevState);
	}
}

void CDragonBHV_GetHit::Exit()
{
}
