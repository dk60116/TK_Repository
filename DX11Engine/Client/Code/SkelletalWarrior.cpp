#include "cpch.h"
#include "SkelletalWarrior.h"

CSkelletalWarrior::CSkelletalWarrior()
	: CMonster{}
{
}

CSkelletalWarrior::~CSkelletalWarrior()
{
}

CSkelletalWarrior* CSkelletalWarrior::Create()
{
	return new CSkelletalWarrior();
}

CComponent* CSkelletalWarrior::Clone() const
{
	CSkelletalWarrior* clone = new CSkelletalWarrior();

	return clone;
}

HRESULT CSkelletalWarrior::Initialize(void* _desc)
{
	m_strMonsterName = L"SkelletalWarrior";

	m_sOptions.scaleFactor = 0.02f;
	m_sOptions.colliderCenter = vector3(0.f, 1.5f, 0.3f);
	m_sOptions.colliderSize = vector3(0.7f, 3.f, 0.7f);
	m_sOptions.headColliderCenter = vector3(0.f, -0.2f, 0.f);
	m_sOptions.headColliderSize = vector3::one() * 0.2f;
	m_sOptions.bodyColliderSize = vector3::one() * 0.5f;

	m_sStatus.maxHp = 5;
	m_sStatus.attackRange = 4.5f;

	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	m_pHeadTF = Get_Transform()->Find_ChildRecursive(L"SkeletonKnight_ Head");
	m_pBodyTF = Get_Transform()->Find_ChildRecursive(L"SkeletonKnight_ Pelvis");

	return S_OK;
}

void CSkelletalWarrior::Awake()
{
	__super::Awake();
}

void CSkelletalWarrior::Start()
{
	__super::Start();
}

void CSkelletalWarrior::Update()
{
	__super::Update();
}

void CSkelletalWarrior::OnDestroy()
{
	__super::OnDestroy();
}
