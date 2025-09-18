#include "cpch.h"
#include "Goblin.h"

CGoblin::CGoblin()
	: CMonster{}
{
}

CGoblin::~CGoblin()
{
}

CGoblin* CGoblin::Create()
{
    return new CGoblin();
}

CComponent* CGoblin::Clone() const
{
	CGoblin* clone = new CGoblin();

	return clone;
}

HRESULT CGoblin::Initialize(void* _desc)
{
	m_strMonsterName = L"Goblin";
	m_sOptions.scaleFactor = 0.015f;
	m_sOptions.colliderCenter = vector3(0.f, 1.f, 0.f);
	m_sOptions.colliderSize = vector3(0.5f, 2.f, 0.5f);
	m_sOptions.headColliderCenter = vector3::right() * 0.1f;
	m_sOptions.headColliderSize = vector3::one() * 0.3f;
	m_sOptions.bodyColliderCenter = vector3(0.f, 0.f, 0.f);
	m_sOptions.bodyColliderSize = vector3(1.5f, 0.5f, 0.5f);

	m_sStatus.maxHp = 5;
	m_sStatus.attackRange = 3.f;

	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	m_pHeadTF = Get_Transform()->Find_ChildRecursive(L"GOBLIN_ Head");
	m_pBodyTF = Get_Transform()->Find_ChildRecursive(L"GOBLIN_ Pelvis");

	return S_OK;
}

void CGoblin::Awake()
{
	__super::Awake();
}

void CGoblin::Start()
{
	__super::Start();
}

void CGoblin::Update()
{
	__super::Update();
}

void CGoblin::OnDestroy()
{
	__super::OnDestroy();
}
