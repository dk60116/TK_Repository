#include "epch.h"
#include "Collider.h"

CCollider::CCollider()
	: m_iColliderID(0)
	, m_mEnteredColliders({})
	, m_bIsTrigger(false)
	, m_pRigid(nullptr)
	, m_bSmoothCollision(false)
	, m_eGizmoColor(GizmoColor::GreenRed)
{
}

CCollider::~CCollider()
{
}

HRESULT CCollider::Initialize(void* _desc)
{
	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	return S_OK;
}

void CCollider::Awake()
{

}

void CCollider::LateUpdate()
{
}

void CCollider::FixedUpdate()
{
	if (m_iColliderID == 0)
		m_iColliderID = m_pGameObject->Get_UniqueID();

	if (!m_pGameObject->IsRecursiveActive() || !m_bEnabled)
		return;

	CCollisionManager::Add_Collider(this);
}

void CCollider::OnEnable()
{
	for (auto& [id, other] : m_mEnteredColliders)
	{
		if (!other)
			continue;

		if (!m_bIsTrigger)
		{
			m_pGameObject->OnCollisionExit(other);
			other->Get_GameObject()->OnCollisionExit(this);
		}
		else {
			m_pGameObject->OnTriggerExit(other);
			other->Get_GameObject()->OnTriggerExit(this);
		}

		other->m_mEnteredColliders.erase(m_iColliderID);
	}

	m_mEnteredColliders.clear();
}

void CCollider::OnDisable()
{
	for (auto& [id, other] : m_mEnteredColliders)
	{
		if (!other)
			continue;
		if (!m_bIsTrigger)
			other->Get_GameObject()->OnCollisionExit(this);
		else
			other->Get_GameObject()->OnTriggerExit(this);

		other->m_mEnteredColliders.erase(m_iColliderID);
	}
	m_mEnteredColliders.clear();
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

const map<_uint, CCollider*>& CCollider::Get_EnteredColliders()
{
	return m_mEnteredColliders;
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

void CCollider::Set_SmoothTranslation(const _bool _value)
{
	m_bSmoothCollision = _value;
}

void CCollider::Set_GizmoColor(const GizmoColor _color)
{
	m_eGizmoColor = _color;
}
