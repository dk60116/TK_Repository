#include "epch.h"
#include "Collider.h"

CCollider::CCollider()
	: m_iColliderID(0)
	, m_mEnteredColliders({})
	, m_bIsTrigger(false)
	, m_pRigid(nullptr)
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

	if (!m_pGameObject->IsActive() || !m_bEnabled)
		return;

	CCollisionManager::Add_Collider(this);
}

void CCollider::OnDestroy()
{
}

const _bool CCollider::IsTrigger() const
{
	return m_bIsTrigger;
}

void CCollider::SetTrigger(const _bool _value)
{
	m_bIsTrigger = _value;
}

CRigidBody* CCollider::Get_RigidBody()
{
	return m_pRigid;
}

void CCollider::Set_RigidBody(CRigidBody* _rigid)
{
	if (_rigid == m_pRigid)
		return;

	m_pRigid = _rigid;
}

void CCollider::EnterOther(CCollider* _other)
{
	if (m_mEnteredColliders.find(_other->m_iColliderID) != m_mEnteredColliders.end())
	{
		if (!m_bIsTrigger)
			m_pGameObject->OnCollisionStay(_other);
		else
			m_pGameObject->OnTriggerStay(_other);
		return;
	}

	m_mEnteredColliders.emplace(_other->m_iColliderID, _other);

	if (!m_bIsTrigger)
		m_pGameObject->OnCollisionEnter(_other);
	else
		m_pGameObject->OnTriggerEnter(_other);
}

void CCollider::ExitOther(CCollider* _other)
{
	auto it = m_mEnteredColliders.find(_other->m_iColliderID);
	
	if (it != m_mEnteredColliders.end())
	{
		if (!m_bIsTrigger)
			Get_GameObject()->OnCollisionExit(_other);
		else
			Get_GameObject()->OnTriggerExit(_other);
		
		m_mEnteredColliders.erase(it);
	}
}
