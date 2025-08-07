#include "epch.h"
#include "NavMeshAgent.h"

using namespace EngineAI;

CNavMeshAgent::CNavMeshAgent()
    : m_fRadius(0.5f)
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
    CPhysics::Ray ray = {};

    CTransform* myTf = Get_Transform();

    ray.origin = myTf->Get_Position() + vector3::up() * 0.5f;
    ray.dir = vector3::down();
   
    auto hits = CPhysics::GetInstance().Raycast(ray, CSceneManager::NameToLayer(L"NaviMesh"));
    CPhysics::RAYCASTHIT hit = {};
    
    if (hits.size() > 0)
    {
        auto hit = hits[0];
        myTf->Set_PositionY(hit.hitPos.y);
    }
}

void CNavMeshAgent::Render()
{
}

void CNavMeshAgent::OnDestroy()
{
}
