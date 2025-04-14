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

    // Transform 설정
    _matrix world = getTransform().getWorldMatrix();
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &world);

    // 카메라 정보 설정
    if (CManagement::GetInstance().getCrtScene()->getCamList().empty())
        return;

    CCamera* pCamera = CManagement::GetInstance().getCrtScene()->getCamera();
    m_pGraphicDev->SetTransform(D3DTS_VIEW, &pCamera->getViewMatrix());
    m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &pCamera->getProjMatrix());

    // 렌더 상태 설정
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // 기본 반시계방향

    // 텍스처나 머티리얼이 있다면 여기서 바인딩
    // ex) m_pMaterial->Bind();

    // 렌더링
    pMesh->Render_Buffer();
}

void CMeshRenderer::OnDestroy()
{
}
