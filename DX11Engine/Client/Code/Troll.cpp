#include "cpch.h"
#include "Troll.h"

CTroll::CTroll()
	: CMonster{}
{
}

CTroll::~CTroll()
{
}

CTroll* CTroll::Create()
{
	return new CTroll();
}

CComponent* CTroll::Clone() const
{
	CTroll* clone = new CTroll();

	return clone;
}

HRESULT CTroll::Initialize(void* _desc)
{
	m_strMonsterName = L"Troll";

	m_sOptions.colliderCenter = vector3(0.f, 1.f, 0.3f);
	m_sOptions.colliderSize = vector3(1.5f, 2.f, 1.5f);
	m_sOptions.headColliderCenter = vector3::right() * 0.2f;
	m_sOptions.headColliderSize = vector3::one() * 0.7f;

	m_sStatus.maxHp = 5;
	m_sStatus.attackRange = 4.5f;

	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	m_pHeadTF = Get_Transform()->Find_ChildRecursive(L"TROLL_ Head");
	m_pBodyTF = Get_Transform()->Find_ChildRecursive(L"TROLL_ Pelvis");

	return S_OK;
}

void CTroll::Awake()
{
	__super::Awake();

	CTransform* chestTF = Get_Transform()->Find_ChildRecursive(L"TROLL_ Spine1");
	CMonsterPartCollision* chestCol = chestTF->Get_GameObject()->AddComponent<CMonsterPartCollision>();
	chestCol->Set_Monster(this);
	chestCol->SetCenter(vector3(0.25f, 0.f, 0.f));
	chestCol->SetSize(vector3(1.5f, 1.2f, 1.2f));
	m_mPartColList.emplace(L"Chest", chestCol);

	CTransform* leftUpperArmTF = Get_Transform()->Find_ChildRecursive(L"TROLL_ L UpperArm");
	CMonsterPartCollision* leftUpperArmCol = leftUpperArmTF->Get_GameObject()->AddComponent<CMonsterPartCollision>();
	leftUpperArmCol->Set_Monster(this);
	leftUpperArmCol->SetCenter(vector3(0.3f, 0.f, 0.f));
	leftUpperArmCol->SetSize(vector3(1.5f, 0.3f, 0.3f));
	m_mPartColList.emplace(L"LeftUpperArm", leftUpperArmCol);

	CTransform* leftForeArmTF = Get_Transform()->Find_ChildRecursive(L"TROLL_ L Forearm");
	CMonsterPartCollision* leftForeArmCol = leftForeArmTF->Get_GameObject()->AddComponent<CMonsterPartCollision>();
	leftForeArmCol->Set_Monster(this);
	leftForeArmCol->SetCenter(vector3(0.5f, 0.f, 0.f));
	leftForeArmCol->SetSize(vector3(1.5f, 0.3f, 0.3f));
	m_mPartColList.emplace(L"LeftForeArm", leftForeArmCol);

	CTransform* leftHandTF = Get_Transform()->Find_ChildRecursive(L"TROLL_ L Hand");
	CMonsterPartCollision* leftHandCol = leftHandTF->Get_GameObject()->AddComponent<CMonsterPartCollision>();
	leftHandCol->Set_Monster(this);
	leftHandCol->SetCenter(vector3(0.3f, 0.f, 0.f));
	leftHandCol->SetSize(vector3::one() * 0.5f);
	m_mPartColList.emplace(L"LeftHand", leftHandCol);

	CTransform* rightUpperArmTF = Get_Transform()->Find_ChildRecursive(L"TROLL_ R UpperArm");
	CMonsterPartCollision* rightUpperArmCol = rightUpperArmTF->Get_GameObject()->AddComponent<CMonsterPartCollision>();
	rightUpperArmCol->Set_Monster(this);
	rightUpperArmCol->SetCenter(vector3(0.3f, 0.f, 0.f));
	rightUpperArmCol->SetSize(vector3(1.5f, 0.3f, 0.3f));
	m_mPartColList.emplace(L"RightUpperArm", rightUpperArmCol);

	CTransform* rightForeArmTF = Get_Transform()->Find_ChildRecursive(L"TROLL_ R Forearm");
	CMonsterPartCollision* rightForeArmCol = rightForeArmTF->Get_GameObject()->AddComponent<CMonsterPartCollision>();
	rightForeArmCol->Set_Monster(this);
	rightForeArmCol->SetCenter(vector3(0.5f, -0.f, 0.f));
	rightForeArmCol->SetSize(vector3(1.5f, 0.3f, 0.3f));
	m_mPartColList.emplace(L"RightForeArm", rightForeArmCol);

	CTransform* rightHandTF = Get_Transform()->Find_ChildRecursive(L"TROLL_ R Hand");
	CMonsterPartCollision* rightHandCol = rightHandTF->Get_GameObject()->AddComponent<CMonsterPartCollision>();
	rightHandCol->Set_Monster(this);
	rightHandCol->SetCenter(vector3(0.3f, 0.f, 0.f));
	rightHandCol->SetSize(vector3::one() * 0.5f);
	m_mPartColList.emplace(L"RightHand", rightHandCol);

	CTransform* leftThighTF = Get_Transform()->Find_ChildRecursive(L"TROLL_ L Thigh");
	CMonsterPartCollision* leftThighCol = leftThighTF->Get_GameObject()->AddComponent<CMonsterPartCollision>();
	leftThighCol->Set_Monster(this);
	leftThighCol->SetCenter(vector3(0.5f, 0.f, 0.f));
	leftThighCol->SetSize(vector3(1.2f, 0.5f, 0.5f));
	m_mPartColList.emplace(L"LeftThigh", leftThighCol);

	CTransform* leftCalfTF = Get_Transform()->Find_ChildRecursive(L"TROLL_ L Calf");
	CMonsterPartCollision* leftCalfCol = leftCalfTF->Get_GameObject()->AddComponent<CMonsterPartCollision>();
	leftCalfCol->Set_Monster(this);
	leftCalfCol->SetCenter(vector3(0.5f, 0.f, 0.f));
	leftCalfCol->SetSize(vector3(1.2f, 0.4f, 0.4f));
	m_mPartColList.emplace(L"LeftCalf", leftCalfCol);

	CTransform* rightThighTF = Get_Transform()->Find_ChildRecursive(L"TROLL_ R Thigh");
	CMonsterPartCollision* rightThighCol = rightThighTF->Get_GameObject()->AddComponent<CMonsterPartCollision>();
	rightThighCol->Set_Monster(this);
	rightThighCol->SetCenter(vector3(0.5f, 0.f, 0.f));
	rightThighCol->SetSize(vector3(1.2f, 0.5f, 0.5f));
	m_mPartColList.emplace(L"rightThigh", rightThighCol);

	CTransform* rightCalfTF = Get_Transform()->Find_ChildRecursive(L"TROLL_ R Calf");
	CMonsterPartCollision* rightCalfCol = rightCalfTF->Get_GameObject()->AddComponent<CMonsterPartCollision>();
	rightCalfCol->Set_Monster(this);
	rightCalfCol->SetCenter(vector3(0.5f, 0.f, 0.f));
	rightCalfCol->SetSize(vector3(1.2f, 0.4f, 0.4f));
	m_mPartColList.emplace(L"RighrCalf", rightCalfCol);
}

void CTroll::Start()
{
	__super::Start();
}

void CTroll::Update()
{
	__super::Update();
}

void CTroll::OnDestroy()
{
	__super::OnDestroy();
}
