#include "cpch.h"
#include "LadderTrigger.h"
#include "Ladder.h"

CLadderTrigger::CLadderTrigger()
	: m_pLadder(nullptr)
	, m_pCollider(nullptr)
{
}

CLadderTrigger::~CLadderTrigger()
{
}

CLadderTrigger* CLadderTrigger::Create()
{
	return new CLadderTrigger();
}

CComponent* CLadderTrigger::Clone() const
{
	CLadderTrigger* clone = new CLadderTrigger();

	return clone;
}

HRESULT CLadderTrigger::Initialize(void* _desc)
{
	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	if (!m_pCollider)
	{
		m_pCollider = m_pGameObject->AddComponent<CBoxCollider>();
	}

	return S_OK;
}

void CLadderTrigger::Awake()
{
}

void CLadderTrigger::Start()
{
}

void CLadderTrigger::Update()
{
}

void CLadderTrigger::OnTriggerEnter(CCollider* _other)
{
}

void CLadderTrigger::OnDestroy()
{
	Safe_Release(m_pLadder);
}

void CLadderTrigger::Set_Ladder(CLadder* _ladder)
{
	m_pLadder = _ladder;

	if (m_pLadder)
		m_pLadder->AddRef();
}
