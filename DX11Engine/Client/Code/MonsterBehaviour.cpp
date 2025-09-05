#include "cpch.h"
#include "MonsterBehaviour.h"

CMonsterBehaviour::CMonsterBehaviour()
	: m_pMonster(nullptr)
	, m_strBehaviourName(L"")
	, m_iWeight(0)
	, m_fPassedTime(0.f)
	, m_bAnyTime(false)
{
}

CMonsterBehaviour::~CMonsterBehaviour()
{
	OnDestroy();
}

HRESULT CMonsterBehaviour::Initialize(CMonster* _monster)
{
	m_pMonster = _monster;

	if (m_pMonster)
		m_pMonster->AddRef();
	else
		return E_FAIL;

	return S_OK;
}

HRESULT CMonsterBehaviour::Initialize(CBossMonster* _boss)
{
	m_pBoss = _boss;

	if (m_pBoss)
		m_pBoss->AddRef();
	else
		return E_FAIL;

	return S_OK;
}

void CMonsterBehaviour::Enter(void* _desc)
{
	m_fPassedTime = 0.f;

	if (m_pMonster)
		m_pMonster->Get_Animator()->Stop();
	if (m_pBoss)
		m_pBoss->Get_Animator()->Stop();
}

void CMonsterBehaviour::During()
{
	m_fPassedTime += DELTA_TIME;
}

void CMonsterBehaviour::Exit()
{
	m_fPassedTime = 0.f;
}

void CMonsterBehaviour::OnDestroy()
{
	Safe_Release(m_pMonster);
	Safe_Release(m_pBoss);
}
