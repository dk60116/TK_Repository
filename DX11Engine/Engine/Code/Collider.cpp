#include "epch.h"
#include "Collider.h"

CCollider::CCollider()
	: m_iColliderID(0)
	, m_mEnteredColliders({})
{
}

CCollider::~CCollider()
{
}

HRESULT CCollider::Initialize()
{
	return S_OK;
}

void CCollider::Awake()
{
	m_iColliderID = m_pGameObject->Get_UniqueID();
}

void CCollider::LateUpdate()
{
}

void CCollider::FixedUpdate()
{
	if (!m_pGameObject->IsActive() || !m_bEnable)
		return;

	CCollisionManager::GetInstance().Add_Collider(this);
}

void CCollider::Render()
{
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
