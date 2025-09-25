#include "cpch.h"
#include "DragonBHV_CombatWait.h"
#include "Dragon.h"

CDragonBHV_CombatWait::CDragonBHV_CombatWait()
    : m_eTurnDir(CMonsterController::TurnDir::None)
    , m_ePrevTurnDir(CMonsterController::TurnDir::None)
{
}

CDragonBHV_CombatWait::~CDragonBHV_CombatWait()
{
}

HRESULT CDragonBHV_CombatWait::Initialize(CMonster* _monster)
{
    m_iWeight = 2;

    if (FAILED(__super::Initialize(_monster)))
        return E_FAIL;

    return S_OK;
}

void CDragonBHV_CombatWait::Enter(void* _desc)
{
    __super::Enter(_desc);

    CDragon* dragon = dynamic_cast<CDragon*>(m_pMonster);

    m_ePrevTurnDir = CMonsterController::TurnDir::None;

    dragon->PlayIdle();
}

void CDragonBHV_CombatWait::During()
{
    __super::During();

    CDragon* dragon = dynamic_cast<CDragon*>(m_pMonster);

    m_fPassedTime += DELTA_TIME;

    CTransform* playerTf = CGameManager::GetInstance().Get_Player()->Get_Transform();
    const vector3 playerPos = playerTf->Get_Position();

    CTransform* myTf = m_pMonster->Get_Transform();
    const vector3 myPos = myTf->Get_Position();

    const _float distance = vector3::Distance(myPos, playerPos);

    quaternion rotQ = myTf->LookQuaternion(playerPos, CTransform::X | CTransform::Z);

    const vector3 fwd = myTf->Get_Directions().forward;
    const vector3 up = myTf->Get_Directions().up;

    vector3 toTarget = (playerPos - myPos);

    if (toTarget.lengthSq() > 1e-8f) 
        toTarget = toTarget.normalized();
    else 
        toTarget = fwd;

    const vector3 c = fwd.cross(toTarget);
    const _float  dot = fwd.dot(toTarget);
    const _float  sgn = c.dot(up);
    _float angleRad = atan2f(sgn, dot);
    _float angleDeg = angleRad * (180.f / XM_PI);

    const _float startThresholdDeg = 20.f; // 이 이상이면 턴 시작/유지
    const _float stopThresholdDeg = 8.f;  // 이 이하이면 턴 중지

    CMonsterController::TurnDir desiredTurn = m_ePrevTurnDir;
    const _float absDeg = fabsf(angleDeg);

    if (absDeg >= startThresholdDeg)
        desiredTurn = (angleDeg > 0.f) ? CMonsterController::TurnDir::Right
        : CMonsterController::TurnDir::Left;
    else if (absDeg <= stopThresholdDeg)
        desiredTurn = CMonsterController::TurnDir::None;

    m_eTurnDir = desiredTurn;

    // 회전 보간
    _float baseSpeed = dragon->Get_Status().rotateSpeed;
    _float turnSpeed =
        (m_eTurnDir == CMonsterController::TurnDir::None) ? 0.f :
        (absDeg < startThresholdDeg ? baseSpeed * 0.5f : baseSpeed * 1.0f);

    _float t = std::clamp(DELTA_TIME * turnSpeed, 0.f, 1.f);

    quaternion curLocal = myTf->Get_LocalQuaternion();
    quaternion blended = quaternion::Slerp(curLocal, rotQ, t);
    myTf->Set_LocalQuaternion(blended);

    // 애니메이션 전환: 상태 변화가 있을 때만
    if (m_eTurnDir != m_ePrevTurnDir)
    {
        m_pMonster->PlayTurn(m_eTurnDir);
    }

    // 공격/추적 전환
    if (m_fPassedTime > m_pMonster->Get_Status().attackWait)
    {
        if (distance > m_pMonster->Get_Status().attackRange)
            dragon->Get_Controller()->ChangeState(CBossController_Dragon::DTracking);
        else
            dragon->Get_Controller()->ChangeState(CBossController_Dragon::Attack_Ground);

        m_fPassedTime = 0.f;
    }

    m_ePrevTurnDir = m_eTurnDir;
}

void CDragonBHV_CombatWait::Exit()
{
    __super::Exit();

    m_ePrevTurnDir = CMonsterController::TurnDir::None;
    m_ePrevTurnDir = CMonsterController::TurnDir::None;
}
