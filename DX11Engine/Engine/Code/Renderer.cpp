#include "epch.h"
#include "Renderer.h"

CRenderer::CRenderer()
	: m_pMaterial(nullptr)
{
}

CRenderer::~CRenderer()
{
}

void CRenderer::OnDestroy()
{
	Safe_Release(m_pMaterial);
}

void CRenderer::Set_Material(CMaterial* pMaterial)
{
	Safe_Release(m_pMaterial);

	m_pMaterial = pMaterial;

	if (m_pMaterial)
		m_pMaterial->AddRef();
}
