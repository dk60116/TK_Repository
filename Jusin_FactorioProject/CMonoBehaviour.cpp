#include "pch.h"
#include "CMonoBehaviour.h"
#include "CGameObject.h"
#include "CSceneManager.h"

CMonoBehaviour::CMonoBehaviour()
	: m_bEnable(true)
	, m_pGameObject(nullptr)
	, m_bIsMouse(false)
	, m_bIsMousePrev(false)
{
}

CMonoBehaviour::~CMonoBehaviour()
{
	OnDisable_();
}

void CMonoBehaviour::MessageUpdate()
{
}

void CMonoBehaviour::Awake_()
{
}

void CMonoBehaviour::Start_()
{
}

void CMonoBehaviour::Reset_()
{
}

void CMonoBehaviour::Update_()
{
	vector2 mousePos = CSceneManager::GetInstance().getCurrentscene()->getMainCamera()->GetMousePosToWorld();

	if (mousePos.x >= getTransform().getPosition().x - getTransform().getScale().x / 2
		&& mousePos.x <= getTransform().getPosition().x + getTransform().getScale().x / 2
		&& mousePos.y > getTransform().getPosition().y - getTransform().getScale().y / 2
		&& mousePos.y < getTransform().getPosition().y + getTransform().getScale().y / 2)
	{
		m_bIsMouse = true;
	}
	else
		m_bIsMouse = false;
}

void CMonoBehaviour::FixedUpdate_()
{
}

void CMonoBehaviour::LateUpdate_()
{
	if (m_bIsMouse && !m_bIsMousePrev)
		OnMouseEnter();
	if (!m_bIsMouse && m_bIsMousePrev)
		OnMouseExit();

	m_bIsMousePrev = m_bIsMouse;
}

void CMonoBehaviour::OnEnable_()
{
}

void CMonoBehaviour::OnDisable_()
{
}

void CMonoBehaviour::OnCollisionEnter_(CCollider* _other)
{
}

void CMonoBehaviour::OnCollisionStay_(CCollider* _other)
{
}

void CMonoBehaviour::OnCollisionExit_(CCollider* _other)
{
}

void CMonoBehaviour::OnDestroy_()
{
	
}

CTransform& CMonoBehaviour::getTransform()
{
	return m_pGameObject->getTransform();
}
