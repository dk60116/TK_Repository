#include "epch.h"
#include "NavMeshAgent.h"

using namespace EngineAI;

CNavMeshAgent::CNavMeshAgent()
    : m_fRadius(0.5f)
    , m_vPrevPosition({})
    , m_vMoveDirection({})
{
}

CNavMeshAgent::~CNavMeshAgent()
{
}

CNavMeshAgent* CNavMeshAgent::Create()
{
    return new CNavMeshAgent();
}

CComponent* CNavMeshAgent::Clone() const
{
    CNavMeshAgent* clone = new CNavMeshAgent();

    return clone;
}

HRESULT CNavMeshAgent::Initialize()
{
    return S_OK;
}

void CNavMeshAgent::Update()
{
    m_vPrevPosition = Get_Transform()->Get_Position();
    Get_Transform()->Update();
}

void CNavMeshAgent::LateUpdate()
{
    RayUpdate_Floor();
    //RayUpdate_Direction();
}

void CNavMeshAgent::Render()
{
}

void CNavMeshAgent::OnDestroy()
{
}

void CNavMeshAgent::RayUpdate_Floor()
{
    CPhysics::Ray ray = {};

    CTransform* myTf = Get_Transform();

    ray.origin = myTf->Get_Position() + vector3::up() * 0.5f;
    ray.dir = vector3::down();
    ray.maxDist = 0.75f;

    auto wallHits = CPhysics::GetInstance().Raycast(ray, CSceneManager::NameToLayer(L"NaviMesh_Walkable"));
    CPhysics::RAYCASTHIT floorHit = {};

    if (!wallHits.empty())
    {
        auto hit = wallHits[0];

        const _float currY = myTf->Get_Position().y;
        const _float targetY = hit.hitPos.y;

        // Y°ª º¸°£
        const _float lerpSpeed = 10.f;
        const _float deltaTime = CTime::GetInstance().Get_DeltaTime();
        const _float newY = Lerp(currY, targetY, deltaTime * lerpSpeed);

        vector3 pos = myTf->Get_Position();
        pos.y = newY;
        myTf->Set_Position(pos);
    }
    else
        myTf->Set_Position(m_vPrevPosition);
}

void CNavMeshAgent::RayUpdate_Direction()
{
    CPhysics::Ray ray = {};

    CTransform* myTf = Get_Transform();

    const vector3 myPos = myTf->Get_Position();

    ray.origin = myPos + vector3::up() * 0.1f;
    ray.dir = (m_vPrevPosition - myPos).normalized();
    ray.maxDist = m_fRadius;

    auto wallHits = CPhysics::GetInstance().Raycast(ray, CSceneManager::NameToLayer(L"NaviMesh_WalkUnable"));
    CPhysics::RAYCASTHIT wallHit = {};

    if (!wallHits.empty())
    {
        myTf->Set_Position(m_vPrevPosition);
    }
}
