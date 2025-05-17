#include "MeshRenderer.h"
#include "EngineEditor.h"
#include "Management.h"
#include "GameObject.h"
#include "Transform.h"
#include "Material.h"

CMeshRenderer::CMeshRenderer()
	: m_pMeshFilter(nullptr)
	, m_pMaterial(nullptr)
{
    m_strName = L"Mesh Renderer";
}

CMeshRenderer::~CMeshRenderer()
{
	OnDestroy();
}

CMeshRenderer* CMeshRenderer::Create()
{
    return new CMeshRenderer();
}

void CMeshRenderer::Init()
{
    __super::Init();

    if (CMeshFilter* mf = m_pGameObject->GetComponent<CMeshFilter>())
        m_pMeshFilter = mf;
    else
        m_pMeshFilter = m_pGameObject->AddComponent<CMeshFilter>();
    
    m_pMeshFilter->AddRef();
    m_pMeshFilter->SetMesh(CMesh::CUBE);

    m_pMaterial = new CMaterial();
    m_pMaterial->AddRef();
}

void CMeshRenderer::Awake()
{
    __super::Awake();
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
    Render_Final(&CManagement::GetInstance().getEditorCamera(), CEngineEditor::GetInstance().getSelectedGameObject() == m_pGameObject);
}

void CMeshRenderer::Render()
{
    __super::Render();
    Render_Final(CManagement::GetInstance().getCrtScene()->getCamera(), false);
}

void CMeshRenderer::OnDestroy()
{
    __super::OnDestroy();
    m_pMaterial->Destroy();
    Safe_Release(m_pMaterial);
}

void CMeshRenderer::Render_Final(CCamera* _camera, _bool _editor)
{
    if (!m_pMeshFilter)
        return;

    CMesh* pMesh = m_pMeshFilter->getMesh();

    if (!pMesh)
        return;

    _matrix world = getTransform().getWorldMatrix();
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &world);

    if (!_camera)
        return;

    _camera->ViewProjection();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

    m_pGraphicDev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

    if (_editor)
        pMesh->Render_Outline(m_pGraphicDev);

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
