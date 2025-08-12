#include "cpch.h"
#include "WoodenSword.h"

CWoodenSword::CWoodenSword()
{
	m_strName = L"WoodenSword";
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

HRESULT CWoodenSword::Initialize()
{
	m_strName = L"WoodenSword";

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_sOptions.localPos = vector3(1.614f, 8.730f, -12.002f);
	m_sOptions.localQuat = quaternion(0.321369f, 0.504944f, 0.568568f, 0.564344f);
	m_sOptions.localScale = 1.5f;

	m_pCollider = m_pGameObject->AddComponent<CBoxCollider>();
	m_pCollider->Set_Center(vector3::up() * 0.4f);
	m_pCollider->Set_Size(vector3(0.1f, 0.75f, 0.05f));

	return S_OK;
}

void CWoodenSword::Awake()
{
	__super::Awake();
}

void CWoodenSword::Start()
{
}

void CWoodenSword::Update()
{
	if (CInput::GetKeyDown(V))
		CDebug::LogError(Get_Transform()->Get_LocalQuaternion());
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
