#include "epch.h"
#include "MeshRenderer.h"

CMeshRenderer::CMeshRenderer()
	: m_pMeshFilter(nullptr)
	, m_pMaterial(nullptr)
{
}

CMeshRenderer::~CMeshRenderer()
{
}

CMeshRenderer* CMeshRenderer::Create()
{
	return new CMeshRenderer();
}

HRESULT CMeshRenderer::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (!m_pMeshFilter)
	{
		m_pMeshFilter = m_pGameObject->AddComponent<CMeshFilter>();

		if (m_pMeshFilter)
			m_pMeshFilter->AddRef();
	}

	D3D11_RASTERIZER_DESC noCullDesc = {};
	noCullDesc.FillMode = D3D11_FILL_SOLID;
	noCullDesc.CullMode = D3D11_CULL_BACK;
	noCullDesc.FrontCounterClockwise = FALSE;
	noCullDesc.DepthClipEnable = TRUE;

	return S_OK;
}

void CMeshRenderer::OnPreCull()
{
}

void CMeshRenderer::OnPreRender()
{
}

void CMeshRenderer::Render()
{
	if (!m_pMeshFilter || !m_pMaterial)
		return;

	// MeshBuffer 가져오기
	CMeshBuffer* pBuffer = m_pMeshFilter->Get_MeshBuffer();
	if (!pBuffer)
		return;

	// World / View / Projection 행렬 계산
	_matrix matWorld = Get_Transform()->Get_WorldMatrix();
	_matrix matView = CSceneManager::GetInstance().Get_CrtScene()->Get_Camera()->Get_ViewMatrix();
	_matrix matProj = CSceneManager::GetInstance().Get_CrtScene()->Get_Camera()->Get_ProjectionMatrix();

	// 셰이더 + 텍스처 + 상수 버퍼 바인딩
	m_pMaterial->Bind(matWorld, matView, matProj);

	//실제 메쉬 렌더링 (버퍼 바인딩 및 Draw)
	pBuffer->Render();
}

void CMeshRenderer::OnPostRender()
{
}

void CMeshRenderer::OnDestroy()
{
	Safe_Release(m_pMeshFilter);
	Safe_Release(m_pMaterial);
}

CMeshFilter* CMeshRenderer::Get_MeshFilter()
{
	return m_pMeshFilter;
}

void CMeshRenderer::Set_Material(CMaterial* pMaterial)
{
	Safe_Release(m_pMaterial);

	m_pMaterial = pMaterial;

	if (m_pMaterial)
		m_pMaterial->AddRef();
}
