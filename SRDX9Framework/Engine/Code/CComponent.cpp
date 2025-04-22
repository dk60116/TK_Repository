#include "CComponent.h"
#include "CGraphicDev.h"
#include "CGameObject.h"

CComponent::CComponent()
	: m_pGameObject(nullptr)
	, m_bEnable(true)
	, m_pGraphicDev(nullptr)
	, m_bClone(false)
{
	SetDevice();
}

CComponent::CComponent(const CComponent& _rhs)
	: m_pGameObject(_rhs.m_pGameObject)
	, m_bEnable(_rhs.m_bEnable)
	, m_pGraphicDev(nullptr)
	, m_bClone(true)
{
	SetDevice();
}

CComponent::~CComponent()
{
	OnDestroy();
	Safe_Release(m_pGraphicDev);
}

void CComponent::Awake()
{
}

void CComponent::Start()
{
}

void CComponent::UpdateEditor()
{
}

void CComponent::Update()
{
}

void CComponent::FixedUpdate()
{
}

void CComponent::LateUpdate()
{
}

void CComponent::RenderEditor()
{
}

void CComponent::Render()
{
}

void CComponent::OnEnable()
{
}

void CComponent::OnDisable()
{
}

void CComponent::OnDestroy()
{
	Release();
}

void CComponent::SetDevice()
{
	m_pGraphicDev = CGraphicDev::GetInstance().Get_GraphicDev();
	m_pGraphicDev->AddRef();
}

CTransform& CComponent::getTransform()
{
	return m_pGameObject->getTransform();
}
