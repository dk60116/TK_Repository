#include "cpch.h"
#include "WoodenSword.h"

CWoodenSword::CWoodenSword()
{
}

CWoodenSword::~CWoodenSword()
{
}

CWoodenSword* CWoodenSword::Create()
{
	return new CWoodenSword();
}

CComponent* CWoodenSword::Clone() const
{
	CWoodenSword* clone = new CWoodenSword();
	
	return clone;
}

void CWoodenSword::PlaySwingSound()
{
	m_pAudioSource->SetClip(CResources::LoadOnScene<CAudioClip>(L"SwingSound (Audio)"));
	m_pAudioSource->Play();
}

HRESULT CWoodenSword::Initialize(void* _desc)
{
	m_sStat.type = CGameManager::WeaponType::Sword;
	m_strWeaponName = L"WoodenSword";

	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	m_sOptions.localPos = vector3(1.614f, 8.73f, -12.f);
	m_sOptions.localQuat = quaternion(0.321369f, 0.504944f, 0.568568f, 0.564344f);
	m_sOptions.localScale = 1.5f;

	m_sStat.attack = 1;
	m_sStat.knockbackPower = 2.f;

	m_pCollider = m_pGameObject->AddComponent<CBoxCollider>();
	CBoxCollider* boxCol = dynamic_cast<CBoxCollider*>(m_pCollider);

	boxCol->Set_Center(vector3::up() * 0.4f);
	boxCol->Set_Size(vector3(0.25f, 1.f, 0.1f));

	return S_OK;
}

void CWoodenSword::Awake()
{
	__super::Awake();
}

void CWoodenSword::Start()
{
	__super::Start();
}

void CWoodenSword::Update()
{
	__super::Update();
}

void CWoodenSword::OnDestroy()
{
}

void CWoodenSword::OnCollisionEnter(CCollider* _other)
{
}

void CWoodenSword::OnCollisionStay(CCollider* _other)
{
}

void CWoodenSword::OnCollisionExit(CCollider* _other)
{
}
