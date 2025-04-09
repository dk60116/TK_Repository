#include "CSpriteRenderer.h"
#include "CGameObject.h"
#include "CManagement.h"

CSpriteRenderer::CSpriteRenderer()
	: m_pTexture(nullptr)
	, m_pBuffer(nullptr)
	, m_sColorTint(ColorValue::white())
	, m_rcUV({})
{
	SetRect(&m_rcUV, 0, 0, 1, 1);
}

CSpriteRenderer::~CSpriteRenderer()
{
	OnDestroy();
}

void CSpriteRenderer::Awake()
{
	m_pBuffer = m_pGameObject->AddComponent<CRectCol>();

	if (FAILED(m_pBuffer->Ready_Buffer()))
		OutputDebugStringA("[SpriteRenderer] Failed to Ready_Buffer()\n");
}

void CSpriteRenderer::Start()
{
}

void CSpriteRenderer::Update()
{
}

void CSpriteRenderer::Render()
{
	if (!m_pBuffer || !m_pGraphicDev)
		return;

	_matrix worldMat = getTransform().getWorldMatrix();

	worldMat._31 = 0.0f;
	worldMat._32 = 0.0f;
	worldMat._33 = 1.0f;
	worldMat._34 = 0.0f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &worldMat);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (m_pTexture)
		m_pGraphicDev->SetTexture(0, m_pTexture);
	else
		m_pGraphicDev->SetTexture(0, nullptr);

	const _matrix& world = getTransform().getWorldMatrix();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &world);
	
	CCamera& cam = *CManagement::GetInstance().getCrtScene()->getCamera(0);

	const _matrix& view = cam.getViewMatrix();
	const _matrix& proj = cam.getProjMatrix();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &view);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &proj);

	m_pBuffer->Render_Buffer();
}

void CSpriteRenderer::OnDestroy()
{
	Safe_Delete(m_pTexture);
}

void CSpriteRenderer::SetTintColor(ColorValue _color)
{
	m_sColorTint = _color;
	m_pBuffer->getOptions().color = m_sColorTint;
	m_pBuffer->UpdateColor();
}
