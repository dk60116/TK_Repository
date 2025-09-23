#include "cpch.h"
#include "Snake.h"

CSnake::CSnake()
	: CMonster{}
{
}

CSnake::~CSnake()
{
}

CSnake* CSnake::Create()
{
    return new CSnake();
}

CComponent* CSnake::Clone() const
{
    CSnake* clone = new CSnake();

    return clone;
}

HRESULT CSnake::Initialize(void* _desc)
{
	m_sOptions.scaleFactor = 0.7f;

	m_strMonsterName = L"Snake";

	m_sOptions.colliderCenter = vector3(0.f, 0.6f, 0.2f);
	m_sOptions.colliderSize = vector3(0.6f, 1.f, 1.5f);
	//m_sOptions.headColliderCenter = vector3::zero();
	m_sOptions.headColliderSize = vector3::one() * 0.3f;
	//m_sOptions.bodyColliderCenter = vector3::right() * 0.5f;
	//m_sOptions.bodyColliderSize = vector3(1.5f, 0.5f, 0.5f);

	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	m_pHeadTF = Get_Transform()->Find_ChildRecursive(L"GIANT_VIPER_ Head");
	//m_pBodyTF = Get_Transform()->Find_ChildRecursive(L"FantasyWolf_ Pelvis");

	return S_OK;
}

void CSnake::Awake()
{
	__super::Awake();
}

void CSnake::Start()
{
	__super::Start();
}

void CSnake::Update()
{
	__super::Update();
}

void CSnake::OnDestroy()
{
	__super::OnDestroy();
}

