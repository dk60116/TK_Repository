#include "cpch.h"
#include "Behaviour_CombatWait.h"

CBehaviour_CombatWait::CBehaviour_CombatWait()
{
}

CBehaviour_CombatWait::~CBehaviour_CombatWait()
{
}

HRESULT CBehaviour_CombatWait::Initialize(CMonster* _monster)
{
    if (FAILED(__super::Initialize(_monster)))
        return E_FAIL;

    return S_OK;
}

void CBehaviour_CombatWait::Enter()
{
    __super::Enter();

    m_pMonster->Get_Animator()->SetLoop(true);
    m_pMonster->Get_Animator()->Play(L"Walk", 0.1f);
}

void CBehaviour_CombatWait::During()
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

    if (m_fPassedTime > m_pMonster->Get_Status().attackWait)
    {
        if (vector3::Distance(myPos, playerPos) > m_pMonster->Get_Status().attackRange)
        {
            m_pMonster->Change_State(CMonsterController::Tracking);
        }
        else
        {
            m_pMonster->Change_State(CMonsterController::Combat);
        }
    }
}

void CBehaviour_CombatWait::Exit()
{
    __super::Exit();
}
