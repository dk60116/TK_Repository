#include "cpch.h"
#include "BossController.h"
#include "BossMonster.h"

#include "DragonBHV_Idle.h"

CBossController::CBossController()
	: m_pMonster(nullptr)
	, m_mBehaviourList({})
	, m_pCrtBehaviour(nullptr)
	, m_bDamaged(false)
	, m_bDead(false)
{
}

CBossController::~CBossController()
{
}

HRESULT CBossController::Initialize(void* _desc)
{
	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	return S_OK;
}

void CBossController::Awake()
{
}

void CBossController::Start()
{
}

void CBossController::Update()
{
	if (m_pCrtBehaviour)
		m_pCrtBehaviour->During();
}

void CBossController::OnDestroy()
{
	Safe_Release(m_pMonster);

	for (TRAVERSAL_ITER(m_mBehaviourList, it))
		Safe_Release((*it).second);

	m_mBehaviourList.clear();
}

void CBossController::Set_Monster(CBossMonster* _monster)
{
	m_pMonster = _monster;

	if (m_pMonster)
		m_pMonster->AddRef();
}

void CBossController::ChangeState(const _uint _state, void* _desc, const _bool _forceEnter)
{
	if (!_forceEnter)
	{
		if (_state == m_iCrtState)
			return;
	}

	if (m_pCrtBehaviour && m_pCrtBehaviour->m_iWeight > m_mBehaviourList[_state]->m_iWeight)
		return;

	if (m_pCrtBehaviour)
		m_pCrtBehaviour->Exit();

	m_pCrtBehaviour = m_mBehaviourList[_state];
	m_pCrtBehaviour->Enter(_desc);

	m_iCrtState = _state;
}

void CBossController::ForceChangeState(const _uint _state, void* _desc)
{
	if (m_pCrtBehaviour)
		m_pCrtBehaviour->Exit();

	m_pCrtBehaviour = m_mBehaviourList[_state];
	m_pCrtBehaviour->Enter(_desc);

	m_iCrtState = _state;
}
