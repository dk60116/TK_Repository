#include "epch.h"
#include "MeshFilter.h"

CMeshFilter::CMeshFilter()
	: m_pMeshBuffer(nullptr)
{
}

CMeshFilter::~CMeshFilter()
{
}

CMeshFilter* CMeshFilter::Create()
{
	return new CMeshFilter();
}

void CMeshFilter::Initialize()
{
	__super::Initialize();
}

void CMeshFilter::Bind_Mesh_Buffer(CMeshBuffer* _buffer)
{
}

CMeshBuffer* CMeshFilter::Get_MeshBuffer() const
{
	return m_pMeshBuffer;
}
