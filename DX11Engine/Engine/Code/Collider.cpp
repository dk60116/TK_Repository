#include "epch.h"
#include "Collider.h"

CCollider::CCollider()
	: m_iColliderID(0)
	, m_mEnteredColliders({})
	, m_bIsTrigger(false)
{
}

CCollider::~CCollider()
{
}

HRESULT CCollider::Initialize()
{
	return S_OK;
}

void CCollider::LateUpdate()
{
}

void CCollider::FixedUpdate()
{
	if (m_iColliderID == 0)
		m_iColliderID = m_pGameObject->Get_UniqueID();

	if (!m_pGameObject->IsActive() || !m_bEnable)
		return;

	CCollisionManager::Add_Collider(this);
}

void CCollider::OnDestroy()
{
}

void CCollider::EnterOther(CCollider* _other)
{
	if (!_other || _other->m_iColliderID == m_iColliderID)
		return;

	if (m_mEnteredColliders.find(_other->m_iColliderID) != m_mEnteredColliders.end())
	{
		m_pGameObject->OnCollisionStay(_other);
		return;
	}

	m_mEnteredColliders.emplace(_other->m_iColliderID, _other);
	m_pGameObject->OnCollisionEnter(_other);
}

void CCollider::ExitOther(CCollider* _other)
{
	if (!_other || _other->m_iColliderID == m_iColliderID)
		return;

	auto it = m_mEnteredColliders.find(_other->m_iColliderID);
	
	if (it != m_mEnteredColliders.end())
	{
		Get_GameObject()->OnCollisionExit(_other);
		m_mEnteredColliders.erase(it);
	}
}
