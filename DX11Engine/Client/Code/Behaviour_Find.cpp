#include "cpch.h"
#include "Behaviour_Find.h"

CBehaviour_Find::CBehaviour_Find()
{
}

CBehaviour_Find::~CBehaviour_Find()
{
}

HRESULT CBehaviour_Find::Initialize(CMonster* _monster)
{
	if (FAILED(__super::Initialize(_monster)))
		return E_FAIL;

	return S_OK;
}

void CBehaviour_Find::Enter(void* _desc)
{
	__super::Enter();

	m_pMonster->Get_Animator()->SetLoop(false);
	m_pMonster->Get_Animator()->Play(L"Find", 0.1f);

	CDebug::Log("Find Enter");
}

void CBehaviour_Find::During()
{
	__super::During();

	CTransform* playerTf = CGameManager::GetInstance().Get_Player()->Get_Transform();
	const vector3 playerPos = playerTf->Get_Position();
	const vector3 playerAngle = playerTf->Get_EulerAngles();

	CTransform* myTf = m_pMonster->Get_Transform();
	const vector3 myPos = myTf->Get_Position();
	const vector3 myAngle = myTf->Get_EulerAngles();

	quaternion rotQ = myTf->LookQuaternion(playerPos, CTransform::X | CTransform::Z);

	myTf->Set_Quaternion(quaternion::Slerp(myTf->Get_Quaternion(), rotQ, DELTA_TIME * m_pMonster->Get_Status().rotateSpeed));

	if (m_pMonster->Get_Animator()->Get_StateInfo().normalizeTime >= 0.9f)
	{
		m_pMonster->Change_State(CMonsterController::Tracking);
	}
}

void CBehaviour_Find::Exit()
{
	__super::Exit();
}
