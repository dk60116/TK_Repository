#include "epch.h"
#include "Component.h"

CComponent::CComponent()
	: m_bEnable(true)
{
}

CComponent::~CComponent()
{
	OnDestroy();
}

void CComponent::Awake()
{
}

void CComponent::Start()
{
}

void CComponent::Update_Editor()
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

void CComponent::OnMouseEnter()
{
}

void CComponent::OnMouseOver()
{
}

void CComponent::OnMouseExit()
{
}

void CComponent::OnMouseDown()
{
}

void CComponent::OnMouseDrag()
{
}

void CComponent::OnMouseUp()
{
}

void CComponent::OnPreCull_Editor()
{
}

void CComponent::OnPreRender_Editor()
{
}

void CComponent::Render_Editor()
{
}

void CComponent::OnPostRender_Editor()
{
}

void CComponent::OnPreCull()
{
}

void CComponent::OnPreRender()
{
}

void CComponent::Render()
{
}

void CComponent::OnPostRender()
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
}

void CComponent::OnApplicationQuit()
{
}

const _bool CComponent::Get_Enable() const
{
	return m_bEnable;
}

void CComponent::Set_Enable(const _bool _enable)
{
	m_bEnable = _enable;
}

