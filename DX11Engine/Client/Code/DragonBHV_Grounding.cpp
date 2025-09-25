#include "cpch.h"
#include "DragonBHV_Grounding.h"
#include "Dragon.h"

CDragonBHV_Grounding::CDragonBHV_Grounding()
{
}

CDragonBHV_Grounding::~CDragonBHV_Grounding()
{
}

HRESULT CDragonBHV_Grounding::Initialize(CMonster* _monster)
{
	m_iWeight = 2;

	if (FAILED(__super::Initialize(_monster)))
		return E_FAIL;

	return S_OK;
}

void CDragonBHV_Grounding::Enter(void* _desc)
{
	__super::Enter(_desc);

	CDragon* dragon = dynamic_cast<CDragon*>(m_pMonster);

	dragon->PlaySpreadGrounding();
}

void CDragonBHV_Grounding::During()
{
	__super::During();

	CDragon* dragon = dynamic_cast<CDragon*>(m_pMonster);

	CTransform* myTF = m_pMonster->Get_Transform();
	const vector3 myPos = myTF->Get_Position();
	const _float myPosY = myPos.y;

	if (myPosY > 0.f)
	{
		myTF->Add_PositionY(-1.5f * DELTA_TIME);

		if (myPosY < 0.2f)
			myTF->Set_PositionY(0.f);
	}
	else
		dragon->Get_Controller()->ChangeState(CBossController_Dragon::DTracking);
}

void CDragonBHV_Grounding::Exit()
{
	__super::Exit();
}
