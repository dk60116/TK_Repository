#include "CSpriteRenderer.h"
#include "CGameObject.h"
#include "CManagement.h"
#include "CDebug.h"

CSpriteRenderer::CSpriteRenderer()
	: m_pTexture(nullptr)
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
}

void CSpriteRenderer::Start()
{
}

void CSpriteRenderer::Update()
{
    CComponent::Update();
}

void CSpriteRenderer::Render()
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
    world._31 = 0.f; world._32 = 0.f; world._33 = 1.f;
    world._43 += static_cast<_float>(-m_sOptions.sortOrder) * 0.001f;
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &world);

    if (CManagement::GetInstance().getCrtScene()->getCamList().empty())
        return;

    CCamera& cam = *CManagement::GetInstance().getCrtScene()->getCamera();
    m_pGraphicDev->SetTransform(D3DTS_VIEW, &cam.getViewMatrix());
    m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &cam.getProjMatrix());

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    if (m_pMaterial)
        m_pMaterial->Apply(m_pGraphicDev);

    m_pGraphicDev->SetTexture(0, m_pTexture ? m_pTexture->getTexture() : nullptr);

    if (!m_sOptions.lighting)
        m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

    m_pBuffer->Render_Buffer(m_pGraphicDev);

    _bool originLight = CManagement::GetInstance().getCrtScene()->getOptions().lighting;
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, originLight);

    m_pGraphicDev->SetTexture(0, nullptr);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CSpriteRenderer::OnDestroy()
{
	CComponent::OnDestroy();
    
    Safe_Release(m_pBuffer);
    Safe_Delete(m_pMaterial);

	m_pTexture = nullptr;
}

void CSpriteRenderer::SetTexture(CTexture* _texture)
{
    m_pTexture = _texture;

    m_pBuffer->Ready_Buffer(m_pGraphicDev);
}

void CSpriteRenderer::SetTintColor(ColorValue _color)
{
    m_pMaterial->getOptions().diffuseColor = _color;
}
