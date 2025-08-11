#include "cpch.h"
#include "Weapon.h"

CWeapon::CWeapon()
	: m_strWeaponName(L"")
	, m_sOptions({})
	, m_pRenderer(nullptr)
	, m_pTargetHand(nullptr)
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

	m_pGameObject->CreateMeshHierachy(CResources::GetInstance().LoadMeshBuffersOnScene(m_strName + L" (MeshBuffer)"));

	return S_OK;
}

void CWeapon::Awake()
{
	CTransform* tf = Get_Transform();

	tf->SetParent(CGameManager::GetInstance().Get_Player()->Get_Hand());
	tf->Set_LocalPosition(m_sOptions.localPos);
	tf->Set_LocalEulerAngles(m_sOptions.localEuler);
	tf->Set_LocalScale(tf->Get_LocalScale() * m_sOptions.localScale);
}
