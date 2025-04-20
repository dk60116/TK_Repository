#include "CMeshRenderer.h"
#include "CManagement.h"
#include "CGameObject.h"
#include "CTransform.h"

CMeshRenderer::CMeshRenderer()
	: m_pMeshFilter(nullptr)
	, m_pMaterial(nullptr)
{
    m_strName = L"MeshRenderer";
}

CMeshRenderer::~CMeshRenderer()
{
	OnDestroy();
}

void CMeshRenderer::Awake()
{
    CComponent::Awake();

    m_pMeshFilter = m_pGameObject->AddComponent<CMeshFilter>();
    m_pMeshFilter->AddRef();
    m_pMeshFilter->SetMesh(CMesh::CUBE);

    m_pMaterial = new CMaterial();
}

void CMeshRenderer::Start()
{
}

void CMeshRenderer::Update()
{
}

void CMeshRenderer::RenderEditor()
{
    CComponent::RenderEditor();
    Render_Final(CManagement::GetInstance().getCrtScene()->getEditorCamera());
}

void CMeshRenderer::Render()
{
    CComponent::Render();
    Render_Final(CManagement::GetInstance().getCrtScene()->getCamera());
}

void CMeshRenderer::OnDestroy()
{
    CComponent::OnDestroy();
    Safe_Delete(m_pMaterial);
}

void CMeshRenderer::Render_Final(CCamera* _camera)
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

    m_pGraphicDev->SetTransform(D3DTS_VIEW, &_camera->getViewMatrix());
    m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &_camera->getProjMatrix());

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

    m_pGraphicDev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

    if (m_pMaterial)
        m_pMaterial->Apply(m_pGraphicDev);

    pMesh->Render_Buffer(m_pGraphicDev);

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDev->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
}

void CMeshRenderer::SetMeshFilterType(CMesh::MeshType _type)
{
    m_pMeshFilter->SetMesh(_type);
}
