#include "cpch.h"
#include "DungeonChest.h"

CDungeonChest::CDungeonChest()
{
}

CDungeonChest::~CDungeonChest()
{
}

CDungeonChest* CDungeonChest::Create()
{
	return new CDungeonChest();
}

CComponent* CDungeonChest::Clone() const
{
	CDungeonChest* clone = new CDungeonChest();

	return clone;
}

HRESULT CDungeonChest::Initialize()
{
	m_strObjName = L"Dungeon_Chest";

	m_sDescription.colliderSize = vector3(1.f, 1.f, 1.f);

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CDungeonChest::Awake()
{
	__super::Awake();
}

void CDungeonChest::Start()
{
	__super::Start();
}

void CDungeonChest::Update()
{
	__super::Start();
}

void CDungeonChest::OnDestroy()
{
	__super::OnDestroy();
}
