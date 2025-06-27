#include "epch.h"
#include "MeshFilter.h"

CMeshFilter::CMeshFilter()
	: m_pMeshBuffer(nullptr)
{
	m_strName = L"Mesh Filter";
}

CMeshFilter::~CMeshFilter()
{
}

void CMeshFilter::OnDestroy()
{
	Safe_Release(m_pMeshBuffer);
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

void CMeshFilter::CreateMeshBuffer(const wstring _shape, const string _path, const _float _scaleFactor)
{
	if (_shape == L"Cube")
		m_pMeshBuffer = CMeshBuffer::CreateCube(this);
	if (_shape == L"Sphere")
		m_pMeshBuffer = CMeshBuffer::CreateSphere(this);
	if (_shape == L"Triangle")
		m_pMeshBuffer = CMeshBuffer::CreateTriangle(this);
	if (_shape == L"Object")
		m_pMeshBuffer = CMeshBuffer::CreateObjectMesh(this, _path, _scaleFactor);
	
	if (m_pMeshBuffer)
		m_pMeshBuffer->AddRef();
}

void CMeshFilter::Set_MeshBuffer(CMeshBuffer* _buffer)
{
	if (m_pMeshBuffer)
		Safe_Release(m_pMeshBuffer);

	m_pMeshBuffer = _buffer;

	if (m_pMeshBuffer)
		m_pMeshBuffer->AddRef();
}

CMeshBuffer* CMeshFilter::Get_MeshBuffer() const
{
	return m_pMeshBuffer;
}
