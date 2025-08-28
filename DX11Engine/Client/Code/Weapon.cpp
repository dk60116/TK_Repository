#include "cpch.h"
#include "Weapon.h"

CWeapon::CWeapon()
	: m_strWeaponName(L"")
	, m_sOptions({})
	, m_sStat({})
	, m_pRenderer(nullptr)
	, m_pTargetHand(nullptr)
	, m_pCollider(nullptr)
	, m_eHandType(CPlayer::HandType::Right)
{
}

CWeapon::~CWeapon()
{
	Safe_Release(m_pTargetHand);
}

HRESULT CWeapon::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_pGameObject->SetLayer(L"PlayerWeapon");
	m_pGameObject->CreateMeshHierachy(CResources::LoadMeshBuffersOnScene(m_strWeaponName + L" (MeshBuffer)"), 0.01f * m_sOptions.localScale);

	return S_OK;
}

void CWeapon::Awake()
{
	CTransform* tf = Get_Transform();

	tf->SetParent(CGameManager::GetInstance().Get_Player()->Get_Hand(m_eHandType));
	tf->Set_LocalPosition(m_sOptions.localPos);
	tf->Set_LocalQuaternion(m_sOptions.localQuat);
	tf->Set_LocalScale(tf->Get_LocalScale() * m_sOptions.localScale);
}

void CWeapon::Start()
{
	if (m_pCollider)
		m_pCollider->SetEnabled(false);
}

const CGameManager::WeaponType CWeapon::Get_WeaponType() const
{
	return m_sStat.type;
}

const wstring& CWeapon::Get_WeaponName()
{
	return m_strWeaponName;
}

const CWeapon::WEAPONSTAT& CWeapon::Get_Stat()
{
	return m_sStat;
}

void CWeapon::OnOffCollider(const _bool _value)
{
	m_pCollider->SetEnabled(_value);
}
