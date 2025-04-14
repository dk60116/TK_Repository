#include "CMeshFilter.h"
#include "CGameObject.h"

CMeshFilter::CMeshFilter()
	: m_pMesh(nullptr)
{
}

CMeshFilter::~CMeshFilter()
{
	OnDestroy();
}

void CMeshFilter::Awake()
{
	CComponent::Awake();

	m_pMesh = m_pGameObject->AddComponent<CMesh>();
}

void CMeshFilter::Start()
{
}

void CMeshFilter::Update()
{
}

void CMeshFilter::Render()
{
}

void CMeshFilter::OnDestroy()
{
	m_pMesh = nullptr;
}

void CMeshFilter::SetMesh(CMesh::MeshType _type)
{
	m_pMesh->SetMeshType(_type);
	m_pMesh->Ready_Mesh();
}
