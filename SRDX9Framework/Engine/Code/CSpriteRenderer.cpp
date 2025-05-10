#include "CSpriteRenderer.h"
#include "CEngineEditor.h"
#include "CGameObject.h"
#include "CManagement.h"
#include "CDebug.h"
#include "CMaterial.h"

CSpriteRenderer::CSpriteRenderer()
	: m_pSprite(nullptr)
	, m_pBuffer(nullptr)
	, m_rcUV({})
    , m_pMaterial(nullptr)
    , m_sOptions({})
{
    m_strName = L"SpriteRenderer";
	SetRect(&m_rcUV, 0, 0, 1, 1);
}

CSpriteRenderer::~CSpriteRenderer()
{
	OnDestroy();
}

void CSpriteRenderer::Awake()
{
    CComponent::Awake();

    m_pBuffer = new CRectCol();
    m_pBuffer->AddRef();

	if (FAILED(m_pBuffer->Ready_Buffer(m_pGraphicDev)))
		OutputDebugStringA("[SpriteRenderer] Failed to Ready_Buffer()\n");

    m_pMaterial = new CMaterial();
    m_pMaterial->AddRef();
}

void CSpriteRenderer::Start()
{
}

void CSpriteRenderer::Update()
{
    CComponent::Update();
}

void CSpriteRenderer::RenderEditor()
{
    CComponent::RenderEditor();
    Render_Final(&CManagement::GetInstance().getEditorCamera(), CEngineEditor::GetInstance().getSelectedGameObject() == m_pGameObject);
}

void CSpriteRenderer::Render()
{
    CComponent::Render();
    Render_Final(CManagement::GetInstance().getCrtScene()->getCamera(), false);
}

void CSpriteRenderer::OnDestroy()
{
	CComponent::OnDestroy();
    
    m_pBuffer->Destroy();
    Safe_Release(m_pBuffer);
    m_pMaterial->Destroy();
    Safe_Release(m_pMaterial);
    m_pMaterial->Release();
}

CSpriteRenderer* CSpriteRenderer::Create()
{
    return new CSpriteRenderer();
}

void CSpriteRenderer::Render_Final(CCamera* _camera, _bool _editor)
{
    if (!m_pBuffer || !m_pGraphicDev)
        return;

    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 1);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphicDev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

    _matrix world = getTransform().getWorldMatrix();
    world._31 = 0.f; 
    world._32 = 0.f;
    world._33 = 1.f;
    world._43 += static_cast<_float>(-m_sOptions.sortOrder) * 0.001f;
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &world);

    if (!_camera)
        return;

    _camera->ViewProjextion();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    if (_editor)
        m_pBuffer->Render_Outline(m_pGraphicDev);

    if (m_pMaterial)
        m_pMaterial->Apply(m_pGraphicDev);

    m_pGraphicDev->SetTexture(0, m_pSprite ? m_pSprite->getTexture() : nullptr);

    if (!m_sOptions.lighting)
        m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

    m_pBuffer->Render_Buffer(m_pGraphicDev);

    _bool originLight = CManagement::GetInstance().getCrtScene()->getOptions().lighting;
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, originLight);

    m_pGraphicDev->SetTexture(0, nullptr);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void CSpriteRenderer::SetTexture(CTexture* _texture)
{
    if (m_pSprite)
        m_pSprite->Release();

    m_pSprite = _texture;
    m_pSprite->AddRef();

    m_pBuffer->Ready_Buffer(m_pGraphicDev);
}

void CSpriteRenderer::SetTintColor(ColorValue _color)
{
    m_pMaterial->getOptions().diffuseColor = _color;
}
