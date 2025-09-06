#include "cpch.h"
#include "DragonBHV_ShootFireBall.h"
#include "Dragon.h"

CDragonBHV_ShootFireBall::CDragonBHV_ShootFireBall()
{
}

CDragonBHV_ShootFireBall::~CDragonBHV_ShootFireBall()
{
}

HRESULT CDragonBHV_ShootFireBall::Initialize(CBossMonster* _boss)
{
	m_iWeight = 1;

	if (FAILED(__super::Initialize(_boss)))
		E_FAIL;

	return S_OK;
}

void CDragonBHV_ShootFireBall::Enter(void* _desc)
{
	__super::Enter();

	m_pBoss->Get_Animator()->SetLoop(true);
	m_pBoss->Get_Animator()->Play(L"FlyIdle", 0.3f);
}

void CDragonBHV_ShootFireBall::During()
{
	__super::During();

	CDragon* dragon = dynamic_cast<CDragon*>(m_pBoss);

	CTransform* playerTf = CGameManager::GetInstance().Get_Player()->Get_Transform();
	const vector3 playerPos = playerTf->Get_Position();
	const vector3 playerAngle = playerTf->Get_EulerAngles();

	CTransform* myTf = m_pBoss->Get_Transform();
	const vector3 myPos = myTf->Get_Position();
	const vector3 myAngle = myTf->Get_EulerAngles();

	quaternion rotQ = myTf->LookQuaternion(playerPos, CTransform::X | CTransform::Z);

	myTf->Set_Quaternion(quaternion::Slerp(myTf->Get_Quaternion(), rotQ, DELTA_TIME * dragon->Get_Status().flyingTurnSpeed));
}

void CDragonBHV_ShootFireBall::Exit()
{
	__super::Exit();
}
