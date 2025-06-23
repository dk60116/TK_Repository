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

HRESULT CMeshFilter::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CMeshFilter::Bind_Mesh_Buffer(CMeshBuffer* _buffer)
{
}

CMeshBuffer* CMeshFilter::Get_MeshBuffer() const
{
	return m_pMeshBuffer;
}
