#include "MeshFilter.h"
#include "GameObject.h"

CMeshFilter::CMeshFilter()
	: m_pMesh(nullptr)
{
	m_strName = L"Mesh Filter";
}

CMeshFilter::~CMeshFilter()
{
	OnDestroy();
}

CMeshFilter* CMeshFilter::Create()
{
	return new CMeshFilter();
}

void CMeshFilter::Init()
{
	__super::Init();

	m_pMesh = new CMesh();
	m_pMesh->AddRef();
}

void CMeshFilter::Awake()
{
	__super::Awake();
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
	__super::OnDestroy();
	Safe_Release(m_pMesh);
	Release();
}

void CMeshFilter::SetMesh(CMesh::MeshType _type)
{
	m_pMesh->SetMeshType(_type);
	m_pMesh->Ready_Mesh(m_pGraphicDev);
}
