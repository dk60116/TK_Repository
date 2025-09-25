#include "cpch.h"
#include "DragonBHV_Tracking.h"
#include "Dragon.h"

CDragonBHV_Tracking::CDragonBHV_Tracking()
{
}

CDragonBHV_Tracking::~CDragonBHV_Tracking()
{
}

HRESULT CDragonBHV_Tracking::Initialize(CMonster* _monster)
{
    m_iWeight = 2;

    if (FAILED(__super::Initialize(_monster)))
        return E_FAIL;

    return S_OK;
}

void CDragonBHV_Tracking::Enter(void* _desc)
{
    __super::Enter(_desc);

	CDragon* dragon = dynamic_cast<CDragon*>(m_pMonster);

	dragon->PlayRun();
	dragon->SetFlying(false);
}

void CDragonBHV_Tracking::During()
{
    __super::During();

	CDragon* dragon = dynamic_cast<CDragon*>(m_pMonster);

	CTransform* playerTf = CGameManager::GetInstance().Get_Player()->Get_Transform();
	const vector3 playerPos = playerTf->Get_Position();
	const vector3 playerAngle = playerTf->Get_EulerAngles();

	CTransform* myTf = m_pMonster->Get_Transform();
	const vector3 myPos = myTf->Get_Position();
	const vector3 myAngle = myTf->Get_EulerAngles();

	quaternion rotQ = myTf->LookQuaternion(playerPos, CTransform::X | CTransform::Z);

	myTf->Set_Quaternion(quaternion::Slerp(myTf->Get_Quaternion(), rotQ, DELTA_TIME * m_pMonster->Get_Status().rotateSpeed));

	myTf->Add_Position(myTf->Get_Directions().forward * dragon->Get_Status().runSpeed * DELTA_TIME);

	if (vector3::Distance(myPos, playerPos) < dragon->Get_Status().attackRange)
	{
		dragon->Get_Controller()->ChangeState(CBossController_Dragon::Attack_Ground);
	}
}

void CDragonBHV_Tracking::Exit()
{
    __super::Exit();
}
