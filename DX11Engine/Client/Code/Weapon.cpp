#include "cpch.h"
#include "Weapon.h"

CWeapon::CWeapon()
	: m_strWeaponName(L"")
	, m_sOptions({})
	, m_pRenderer(nullptr)
	, m_pTargetHand(nullptr)
	, m_pCollider(nullptr)
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
	m_pGameObject->CreateMeshHierachy(CResources::LoadMeshBuffersOnScene(m_strName + L" (MeshBuffer)"));

	return S_OK;
}

void CWeapon::Awake()
{
	CTransform* tf = Get_Transform();

	tf->SetParent(CGameManager::GetInstance().Get_Player()->Get_Hand());
	tf->Set_LocalPosition(m_sOptions.localPos);
	tf->Set_LocalQuaternion(m_sOptions.localQuat);
	tf->Set_LocalScale(tf->Get_LocalScale() * m_sOptions.localScale);

	m_pCollider->Set_Enabled(false);
}

void CWeapon::OnOffCollider(const _bool _value)
{
	m_pCollider->Set_Enabled(_value);
}
