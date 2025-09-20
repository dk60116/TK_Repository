#include "cpch.h"
#include "LadderTrigger.h"
#include "Ladder.h"

CLadderTrigger::CLadderTrigger()
	: m_pLadder(nullptr)
	, m_bOnTrigger(false)
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
	m_strObjName = L"LadderTrigger";

	m_sDescription.isTrigger = true;
	m_sDescription.colliderSize = vector3(0.7f, 0.7f, 0.2f);

	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	m_pGameObject->SetTag(L"ArrowTrigger");

	return S_OK;
}

void CLadderTrigger::Awake()
{
	__super::Awake();
}

void CLadderTrigger::Start()
{
	__super::Start();
}

void CLadderTrigger::Update()
{
	__super::Update();

	if (CInput::GetKeyDown_Editor(U))
		m_bOnTrigger = true;

	if (m_bOnTrigger)
	{
		CTransform* ladderTF = m_pLadder->Get_Transform()->Get_Parent();

		const _float rotZ = ladderTF->Get_EulerAngles().z;
		
		if (abs(rotZ) > 1.f)
			ladderTF->Add_EulerAnglesZ(-90.f * DELTA_TIME);
	}
}

void CLadderTrigger::OnTriggerEnter(CCollider* _other)
{
	if (_other->Get_GameObject()->CompareTag(L"Arrow"))
	{
		m_bOnTrigger = true;
	}
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
