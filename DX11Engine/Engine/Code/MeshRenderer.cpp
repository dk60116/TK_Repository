#include "epch.h"
#include "MeshRenderer.h"
#include "MeshFilter.h"

MeshRenderer::MeshRenderer()
{
}

MeshRenderer::~MeshRenderer()
{
}

HRESULT MeshRenderer::Initialize()
{
	return S_OK;
}

void MeshRenderer::OnPreCull()
{
}

void MeshRenderer::OnPreRender()
{
}

void MeshRenderer::Render()
{
	//if (!m_pMeshFilter)
	//	return;

 //   // 디바이스에 셰이더 세팅
 //   m_pDevice->VSSetShader(m_pDefaultVS, nullptr, 0);
 //   m_pContext->PSSetShader(m_pDefaultPS, nullptr, 0);

 //   // 상수 버퍼(world/view/proj) 세팅
 //   m_pContext->UpdateSubresource(m_pMatrixBuffer, 0, nullptr, &matrices, 0, 0);
 //   m_pContext->VSSetConstantBuffers(0, 1, &m_pMatrixBuffer);

 //   // 메시 렌더링
 //   m_pMeshFilter->Render();
}

void MeshRenderer::OnPostRender()
{
}

void MeshRenderer::Set_MeshFilter(CMeshFilter* pFilter)
{
	m_pMeshFilter = pFilter;
}

void MeshRenderer::Set_Material(CMaterial* pMaterial)
{
}
