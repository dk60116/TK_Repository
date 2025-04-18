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

	m_pMesh = new CMesh();
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
	CComponent::OnDestroy();
	Safe_Delete(m_pMesh);
}

void CMeshFilter::SetMesh(CMesh::MeshType _type)
{
	m_pMesh->SetMeshType(_type);
	m_pMesh->Ready_Mesh(m_pGraphicDev);
}
