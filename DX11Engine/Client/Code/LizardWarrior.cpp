#include "cpch.h"
#include "LizardWarrior.h"

CLizardWarrior::CLizardWarrior()
	: CMonster{}
{
}

CLizardWarrior::~CLizardWarrior()
{
}

CLizardWarrior* CLizardWarrior::Create()
{
	return new CLizardWarrior();
}

CComponent* CLizardWarrior::Clone() const
{
	CLizardWarrior* clone = new CLizardWarrior();

	return clone;
}

HRESULT CLizardWarrior::Initialize(void* _desc)
{
	m_strMonsterName = L"LizardWarrior";

	m_sOptions.colliderCenter = vector3(0.f, 0.6f, 0.2f);
	m_sOptions.colliderSize = vector3(0.5f, 1.f, 1.5f);

	m_sStatus.maxHp = 5;
	m_sStatus.attackRange = 4.5f;

	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	m_pHeadTF = Get_Transform()->Find_ChildRecursive(L"LizardWarrior_ Head");

	return S_OK;
}

void CLizardWarrior::Awake()
{
	__super::Awake();
}

void CLizardWarrior::Start()
{
	__super::Start();
}

void CLizardWarrior::Update()
{
	__super::Update();
}

void CLizardWarrior::OnDestroy()
{
	__super::OnDestroy();
}
