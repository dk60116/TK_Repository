#include "cpch.h"
#include "FootSwitch.h"
#include "DungeonGate.h"

CFootSwitch::CFootSwitch()
{
}

CFootSwitch::~CFootSwitch()
{
}

CFootSwitch* CFootSwitch::Create()
{
	return new CFootSwitch();
}

CComponent* CFootSwitch::Clone() const
{
	CFootSwitch* clone = new CFootSwitch();

	return clone;
}

HRESULT CFootSwitch::Initialize()
{
	m_strObjName = L"Dungeon_FootSwitchPlat";

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CFootSwitch::Awake()
{
	__super::Awake();
}

void CFootSwitch::Update()
{
	__super::Update();
}

void CFootSwitch::OnDestroy()
{
	__super::OnDestroy();
}

void CFootSwitch::Set_Gate(vector<class CDungeonGate*>& _gates)
{
	m_pGates = _gates;

	for (TRAVERSAL_ITER(m_pGates, it))
	{
		if (*it)
			(*it)->AddRef();
	}
}
