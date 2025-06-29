#include "epch.h"
#include "SkinnedMeshRenderer.h"

CSkinnedMeshRenderer::CSkinnedMeshRenderer()
	: m_pMeshBuffer(nullptr)
	, m_vBones({})
	, m_pRootBone(nullptr)
	, m_pBoneMatrixBuffer(nullptr)
	, m_iCBSize(0)
{
	m_strName = L"Skinned Mesh Renderer";
}

CSkinnedMeshRenderer::~CSkinnedMeshRenderer()
{
}

CSkinnedMeshRenderer* CSkinnedMeshRenderer::Create()
{
	return new CSkinnedMeshRenderer();
}

HRESULT CSkinnedMeshRenderer::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CSkinnedMeshRenderer::OnPreCull()
{
}

void CSkinnedMeshRenderer::OnPreRender()
{
}

void CSkinnedMeshRenderer::Render_Editor()
{
	Render_WithCamera(CSceneManager::GetInstance().Get_CrtScene()->Get_EditorCamera());
}

void CSkinnedMeshRenderer::Render()
{
	Render_WithCamera(CSceneManager::GetInstance().Get_CrtScene()->Get_Camera());
}

void CSkinnedMeshRenderer::OnPostRender()
{
}

void CSkinnedMeshRenderer::OnDestroy()
{
	__super::OnDestroy();

	Safe_Release(m_pBoneMatrixBuffer);
	Safe_Release(m_pMeshBuffer);
}

void CSkinnedMeshRenderer::Render_WithCamera(CCamera* _cam)
{
}

void CSkinnedMeshRenderer::SetMesh(CMeshBuffer* _mesh)
{
	Safe_Release(m_pMeshBuffer);

	m_pMeshBuffer = dynamic_cast<CSkinnedMeshBuffer*>(_mesh);

	if (m_pMeshBuffer)
		m_pMeshBuffer->AddRef();
	else
	{
		m_pMeshBuffer = nullptr;
		return;
	}
}
