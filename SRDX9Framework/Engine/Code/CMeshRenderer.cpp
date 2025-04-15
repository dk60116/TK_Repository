#include "CMeshRenderer.h"
#include "CManagement.h"

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

    // 텍스처나 머티리얼이 있다면 여기서 바인딩
    // ex) m_pMaterial->Bind();

    pMesh->Render_Buffer();
}

void CMeshRenderer::OnDestroy()
{
}
