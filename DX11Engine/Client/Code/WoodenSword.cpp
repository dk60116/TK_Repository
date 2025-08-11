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

	m_sOptions.localPos = vector3(5.55f, 9.54f, -10.82f);
	m_sOptions.localEuler = vector3(-5.59f, 93.3f, 83.3f);
	m_sOptions.localScale = 1.5f;

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
