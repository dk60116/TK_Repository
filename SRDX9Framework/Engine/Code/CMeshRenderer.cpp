#include "CMeshRenderer.h"
#include "CManagement.h"
#include "CTransform.h"

CMeshRenderer::CMeshRenderer()
	: m_pMeshFilter(nullptr)
	, m_pMaterial(nullptr)
{
}

CMeshRenderer::~CMeshRenderer()
{
	OnDestroy();
}

void CMeshRenderer::Awake()
{
    CComponent::Awake();

    m_pMaterial = new CMaterial();
}

void CMeshRenderer::Start()
{
}

void CMeshRenderer::Update()
{
}

void CMeshRenderer::Render()
{
    if (!m_pMeshFilter)
        return;

    CMesh* pMesh = m_pMeshFilter->getMesh();

    if (!pMesh)
        return;

    _matrix world = getTransform().getWorldMatrix();
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &world);

    if (CManagement::GetInstance().getCrtScene()->getCamList().empty())
        return;

    CCamera* pCamera = CManagement::GetInstance().getCrtScene()->getCamera();

    m_pGraphicDev->SetTransform(D3DTS_VIEW, &pCamera->getViewMatrix());
    m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &pCamera->getProjMatrix());

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    m_pGraphicDev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

    if (m_pMaterial)
        m_pMaterial->Apply(m_pGraphicDev);

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDev->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_SPECULARENABLE, FALSE);

    pMesh->Render_Buffer();
}

void CMeshRenderer::OnDestroy()
{
    Safe_Delete(m_pMaterial);
}
