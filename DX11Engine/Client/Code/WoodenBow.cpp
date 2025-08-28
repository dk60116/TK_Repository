#include "cpch.h"
#include "WoodenBow.h"

CWoodenBow::CWoodenBow()
{
}

CWoodenBow::~CWoodenBow()
{
}

CWoodenBow* CWoodenBow::Create()
{
    return new CWoodenBow();
}

CComponent* CWoodenBow::Clone() const
{
    CWoodenBow* clone = new CWoodenBow();

	return clone;
}

HRESULT CWoodenBow::Initialize()
{
	m_sStat.type = CGameManager::WeaponType::Bow;
	m_strWeaponName = L"WoodenBow";
	m_eHandType = CPlayer::HandType::Left;

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_sOptions.localPos = vector3(-4.55f, -4.85f, -2.25f);
	m_sOptions.localQuat = quaternion(-0.329272f, 0.711274f, -0.278215f, 0.555217f);
	m_sOptions.localScale = 0.8f;

	m_sStat.attack = 1;
	m_sStat.knockbackPower = 2.f;

	return S_OK;
}

void CWoodenBow::Awake()
{
	__super::Awake();
}

void CWoodenBow::Start()
{
}

void CWoodenBow::Update()
{
}

void CWoodenBow::OnDestroy()
{
}

void CWoodenBow::OnCollisionEnter(CCollider* _other)
{
}

void CWoodenBow::OnCollisionStay(CCollider* _other)
{
}

void CWoodenBow::OnCollisionExit(CCollider* _other)
{
}
