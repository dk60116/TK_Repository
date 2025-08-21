#include "cpch.h"
#include "Wolf.h"

CWolf::CWolf()
	: CMonster{}
{
}

CWolf::~CWolf()
{
}

CWolf* CWolf::Create()
{
	return new CWolf();
}

CComponent* CWolf::Clone() const
{
	CWolf* clone = new CWolf();

	return clone;
}

HRESULT CWolf::Initialize()
{
	m_strMonsterName = L"Wolf";

	m_sOptions.colliderCenter = vector3(0.f, 0.6f, 0.2f);
	m_sOptions.colliderSize = vector3(0.6f, 1.f, 1.5f);
	m_sOptions.headColliderCenter = vector3::zero();
	m_sOptions.headColliderSize = vector3::one() * 0.5f;
	m_sOptions.bodyColliderCenter = vector3::right() * 0.5f;
	m_sOptions.bodyColliderSize = vector3(1.5f, 0.5f, 0.5f);

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_pHeadTF = Get_Transform()->Find_ChildRecursive(L"FantasyWolf_ Head");
	m_pBodyTF = Get_Transform()->Find_ChildRecursive(L"FantasyWolf_ Pelvis");

	return S_OK;
}

void CWolf::Awake()
{
	__super::Awake();
}

void CWolf::Start()
{
	__super::Start();
}

void CWolf::Update()
{
	__super::Update();
}

void CWolf::OnDestroy()
{
	__super::OnDestroy();
}
