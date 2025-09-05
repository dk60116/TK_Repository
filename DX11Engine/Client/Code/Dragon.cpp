#include "cpch.h"
#include "Dragon.h"

CDragon::CDragon()
{
}

CDragon::~CDragon()
{
}

CDragon* CDragon::Create()
{
	return new CDragon();
}

CComponent* CDragon::Clone() const
{
	CDragon* clone = new CDragon();

	return clone;
}

HRESULT CDragon::Initialize()
{
	m_strBossName = L"Dragon";

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CDragon::Awake()
{
	__super::Awake();
}

void CDragon::Start()
{
	__super::Start();
}

void CDragon::Update()
{
	__super::Update();
}

void CDragon::OnEnable()
{
	__super::OnEnable();
}

void CDragon::OnCollisionEnter(CCollider* _other)
{
	__super::OnCollisionEnter(_other);
}

void CDragon::OnDestroy()
{
	__super::OnDestroy();
}
