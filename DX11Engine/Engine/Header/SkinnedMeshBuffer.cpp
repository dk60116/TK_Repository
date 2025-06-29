#include "epch.h"
#include "SkinnedMeshBuffer.h"

CSkinnedMeshBuffer::CSkinnedMeshBuffer()
{
	m_strName = L"Skinned Mesh Buffer";
}

CSkinnedMeshBuffer::~CSkinnedMeshBuffer()
{
    OnDestroy();
}

CSkinnedMeshBuffer* CSkinnedMeshBuffer::Create()
{
	return new CSkinnedMeshBuffer();
}

HRESULT CSkinnedMeshBuffer::Initialize(const string& _filePath, float _scaleFactor)
{
    return S_OK;
}

void CSkinnedMeshBuffer::Render()
{
}

void CSkinnedMeshBuffer::OnDestroy()
{
}
