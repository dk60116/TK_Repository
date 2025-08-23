#include "cpch.h"
#include "Arrow.h"

CArrow::CArrow()
	: m_pRigidBody(nullptr)
	, m_bUsed(false)
	, m_fLifeTime(10.f)
	, m_fPassedTime(0.f)
{
	m_strName = L"Arrow";
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
	spCol->Set_Size(2.f);

	m_pCollider->SetEnabled(false);

	m_pRigidBody = m_pGameObject->AddComponent<CRigidBody>();
	m_pRigidBody->SetKinematic(true);
	m_pRigidBody->SetEnabled(false);

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

		if (m_fPassedTime >= m_fLifeTime)
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

	CTransform* playerTF = CGameManager::GetInstance().Get_Player()->Get_Transform();
	CPlayerCamera* playerCam = CGameManager::GetInstance().Get_PlayerCamera();

	_float posY = Get_Transform()->Get_Position().y;
	vector3 target = playerTF->Get_Position() + playerTF->Get_Directions().forward * 3.f;
	target.y = posY + playerCam->Get_BowY() + 0.3f;

	vector3 myPos = playerTF->Get_Position() + playerTF->Get_Directions().forward;
	myPos.y = posY;

	Get_Transform()->Set_Position(myPos);

	vector3 dir = (target - myPos).normalized();

	Get_Transform()->Set_Quaternion(playerCam->Get_Transform()->Get_Quaternion());
	Get_Transform()->Add_LocalEulerAnglesX(90.f);

	CGameObject* t = m_pGameObject->Get_Scene()->Add_GameObject(L"Tests");
	t->Get_Transform()->Set_Position(target);

	m_pRigidBody->SetEnabled(true);
	m_pRigidBody->SetUseGravity(true);
	m_pRigidBody->SetKinematic(false);
	m_pRigidBody->AddForce(dir * 50.f);
}

void CArrow::Return()
{
	m_bUsed = false;

	m_pRigidBody->SetKinematic(true);
	m_pRigidBody->SetEnabled(false);

	Get_Transform()->SetParent(CGameManager::GetInstance().Get_Player()->Get_Hand(CPlayer::HandType::Right));
	Get_Transform()->Set_LocalPosition(m_sOptions.localPos);
	Get_Transform()->Set_LocalQuaternion(m_sOptions.localQuat);
	m_pGameObject->SetActive(false);

	auto& container = CGameManager::GetInstance().Get_Player()->Get_ArrowContainer(m_strWeaponName);

	container.push(this);
}
