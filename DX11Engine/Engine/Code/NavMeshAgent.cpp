#include "epch.h"
#include "NavMeshAgent.h"

CNavMeshAgent::CNavMeshAgent()
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
}

void CNavMeshAgent::Render()
{
}

void CNavMeshAgent::OnDestroy()
{
}
