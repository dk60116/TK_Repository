#include "CSpriteRenderer.h"
#include "CGameObject.h"
#include "CManagement.h"
#include "CDebug.h"

CSpriteRenderer::CSpriteRenderer()
	: m_pTexture(nullptr)
	, m_pBuffer(nullptr)
	, m_sColorTint(ColorValue::white())
	, m_rcUV({})
    , m_iSotOrdr(0)
{
	SetRect(&m_rcUV, 0, 0, 1, 1);
}

CSpriteRenderer::~CSpriteRenderer()
{
	OnDestroy();
}

void CSpriteRenderer::Awake()
{
    CComponent::Awake();

	m_pBuffer = m_pGameObject->AddComponent<CRectCol>();

	if (FAILED(m_pBuffer->Ready_Buffer()))
		OutputDebugStringA("[SpriteRenderer] Failed to Ready_Buffer()\n");
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

    _matrix world = getTransform().getWorldMatrix();

    world._31 = 0.0f;
    world._32 = 0.0f;
    world._33 = 1.0f;

    world._43 = static_cast<_float>(-m_iSotOrdr) * 0.001f;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, &world);

    if (CManagement::GetInstance().getCrtScene()->getCamList().size() <= 0)
        return;

    CCamera& cam = *CManagement::GetInstance().getCrtScene()->getCamera(0);
    m_pGraphicDev->SetTransform(D3DTS_VIEW, &cam.getViewMatrix());
    m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &cam.getProjMatrix());

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    if (m_pTexture)
        m_pGraphicDev->SetTexture(0, m_pTexture->getTexture());
    else
        m_pGraphicDev->SetTexture(0, nullptr);

    m_pBuffer->Render_Buffer();
}

void CSpriteRenderer::OnDestroy()
{
	CComponent::OnDestroy();
	m_pTexture = nullptr;
}

void CSpriteRenderer::SetTexture(CTexture* _texture)
{
    m_pTexture = _texture;

    if (m_pTexture)
    {
        m_pBuffer->SetRenderType(CVIBuffer::Buffer_Texture);
        m_pBuffer->Ready_Buffer();
    }
    else
        m_pBuffer->SetRenderType(CVIBuffer::Buffer_Color);
}

void CSpriteRenderer::SetTintColor(ColorValue _color)
{
	m_sColorTint = _color;

    if (m_pBuffer)
    {
        m_pBuffer->getOptions().color = m_sColorTint;
        m_pBuffer->UpdateColor();
    }
}
