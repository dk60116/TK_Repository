#include "cpch.h"
#include "DefaultArrow.h"

CDefaultArrow::CDefaultArrow()
{
}

CDefaultArrow::~CDefaultArrow()
{
}

CDefaultArrow* CDefaultArrow::Create()
{
	return new CDefaultArrow();
}

CComponent* CDefaultArrow::Clone() const
{
	CDefaultArrow* clone = new CDefaultArrow();

	return clone;
}

HRESULT CDefaultArrow::Initialize(void* _desc)
{
	m_sStat.type = CGameManager::WeaponType::Arrow;
	m_strWeaponName = L"DefaultArrow";
	m_eHandType = CPlayer::HandType::Right;

	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	m_sOptions.localPos = vector3(1.345f, 0.43f, -0.427f);
	m_sOptions.localQuat = quaternion(0.146432f, -0.563549f, -0.047580f, 0.811608f);
	m_sOptions.localScale = 0.1f;

	m_sStat.attack = 1;
	m_sStat.knockbackPower = 2.f;

	return S_OK;
}

void CDefaultArrow::Awake()
{
	__super::Awake();
}

void CDefaultArrow::Start()
{
	__super::Start();
}

void CDefaultArrow::Update()
{
	__super::Update();

	if (CInput::GetKeyDown(V))
		CDebug::LogError(Get_Transform()->Get_LocalQuaternion());
}

void CDefaultArrow::OnDestroy()
{
	__super::OnDestroy();
}

void CDefaultArrow::OnCollisionEnter(CCollider* _other)
{
	__super::OnCollisionEnter(_other);
}

void CDefaultArrow::OnCollisionStay(CCollider* _other)
{
	__super::OnCollisionStay(_other);
}

void CDefaultArrow::OnCollisionExit(CCollider* _other)
{
	__super::OnCollisionExit(_other);
}
