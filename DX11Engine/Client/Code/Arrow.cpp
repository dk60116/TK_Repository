#include "cpch.h"
#include "Arrow.h"

CArrow::CArrow()
	: m_pRigidBody(nullptr)
	, m_bUsed(false)
	, m_fLiveTime(10.f)
	, m_fPassedTime(0.f)
{
}

CArrow::~CArrow()
{
}

CComponent* CArrow::Clone() const
{
	return nullptr;
}

HRESULT CArrow::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_pCollider = m_pGameObject->AddComponent<CSphereCollider>();
	CSphereCollider* spCol = dynamic_cast<CSphereCollider*>(m_pCollider);

	spCol->SetTrigger(true);
	spCol->Set_Center(vector3(0.f, 4.5f, 0.f));

	m_pRigidBody = m_pGameObject->AddComponent<CRigidBody>();

	return S_OK;
}

void CArrow::Awake()
{
	__super::Awake();
}

void CArrow::Start()
{
	__super::Start();
}

void CArrow::Update()
{
	__super::Update();

	if (m_bUsed)
	{
		m_fPassedTime += DELTA_TIME;

		if (m_fPassedTime >= m_fLiveTime)
			Return();
	}
}

void CArrow::OnDestroy()
{

}

void CArrow::OnCollisionEnter(CCollider* _other)
{
	__super::OnCollisionEnter(_other);
}

void CArrow::OnCollisionStay(CCollider* _other)
{
	__super::OnCollisionStay(_other);
}

void CArrow::OnCollisionExit(CCollider* _other)
{
	__super::OnCollisionExit(_other);
}

const _bool CArrow::IsUsed() const
{
	return m_bUsed;
}

void CArrow::Pop()
{
	m_pGameObject->SetActive(true);
	Get_Transform()->Set_LocalScale(10.f);
}

void CArrow::Shoot()
{
	m_bUsed = true;
	Get_Transform()->SetParent(nullptr);
	m_fPassedTime = 0.f;

	vector3 pos = Get_Transform()->Get_Position();
	vector3 target = CGameManager::GetInstance().Get_PlayerCamera()->Get_FinalTargetPos() + vector3::down() * 1.25f;

	vector3 dir = (target - pos).normalized();

	CGameObject* t = m_pGameObject->Get_Scene()->Add_GameObject(L"Tests");
	t->Get_Transform()->Set_Position(target);

	m_pRigidBody->AddForce(dir * 30.f);
}

void CArrow::Return()
{
	m_bUsed = false;
}
