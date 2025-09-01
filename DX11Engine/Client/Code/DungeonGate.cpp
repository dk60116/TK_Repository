#include "cpch.h"
#include "DungeonGate.h"

CDungeonGate::CDungeonGate()
	: m_iChapterIndex({})
{
}

CDungeonGate::~CDungeonGate()
{
}

CDungeonGate* CDungeonGate::Create()
{
	return new CDungeonGate();
}

CComponent* CDungeonGate::Clone() const
{
	CDungeonGate* clone = new CDungeonGate();

	return clone;
}

HRESULT CDungeonGate::Initialize()
{
	m_strObjName = L"Dungeon_Gate";
	m_bHasAnimation = false;

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_pGameObject->SetLayer(CSceneManager::NameToLayer(L"Map"));

	return S_OK;
}

void CDungeonGate::Awake()
{
	__super::Awake();
}

void CDungeonGate::Update()
{
	__super::Update();
}

void CDungeonGate::OnDestroy()
{
	__super::OnDestroy();
}
