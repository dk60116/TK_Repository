#include "cpch.h"
#include "MonsterController.h"
#include "Monster.h"
#include "Behaviour_Idle.h"
#include "Behaviour_Patrole.h"
#include "Behaviour_Tracking.h"
#include "Behaviour_Combat.h"
#include "Behaviour_Find.h"
#include "Behaviour_CombatWait.h"
#include "Behaviour_GetHit.h"
#include "Behaviour_Death.h"
#include "PlayerController.h"
#include "BossController.h"

CMonsterController::CMonsterController()
	: m_pMonster(nullptr)
	, m_iCrtState(0)
	, m_mBehaviourList({})
	, m_pCrtBehaviour(nullptr)
	, m_bDamaged(false)
	, m_bDead(false)
{
}

CMonsterController::~CMonsterController()
{
}

CMonsterController* CMonsterController::Create()
{
	return new CMonsterController();
}

CComponent* CMonsterController::Clone() const
{
	CMonsterController* clone = new CMonsterController();

	return clone;
}

HRESULT CMonsterController::Initialize(void* _desc)
{
	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	return S_OK;
}

void CMonsterController::Awake()
{
	CBehaviour_Idle* idle = new CBehaviour_Idle();
	idle->Initialize(m_pMonster);
	idle->AddRef();
	m_mBehaviourList.emplace(Idle, idle);

	CBehaviour_Patrole* patrole = new CBehaviour_Patrole();
	patrole->Initialize(m_pMonster);
	patrole->AddRef();
	m_mBehaviourList.emplace(Patrole, patrole);

	CBehaviour_Find* find = new CBehaviour_Find();
	find->Initialize(m_pMonster);
	find->AddRef();
	m_mBehaviourList.emplace(Find, find);

	CBehaviour_Tracking* tracking = new CBehaviour_Tracking();
	tracking->Initialize(m_pMonster);
	tracking->AddRef();
	m_mBehaviourList.emplace(Tracking, tracking);

	CBehaviour_Combat* combat = new CBehaviour_Combat();
	combat->Initialize(m_pMonster);
	combat->AddRef();
	m_mBehaviourList.emplace(Combat, combat);

	CBehaviour_CombatWait* combatWait = new CBehaviour_CombatWait();
	combatWait->Initialize(m_pMonster);
	combatWait->AddRef();
	m_mBehaviourList.emplace(CombatWait, combatWait);

	CBehaviour_GetHit* getHit = new CBehaviour_GetHit();
	getHit->Initialize(m_pMonster);
	getHit->AddRef();
	m_mBehaviourList.emplace(GetHit, getHit);

	CBehaviour_Death* death = new CBehaviour_Death();
	death->Initialize(m_pMonster);
	death->AddRef();
	m_mBehaviourList.emplace(Death, death);

	if (m_pMonster)
	{
		ChangeState(Idle);
	}
}

void CMonsterController::Start()
{
}

void CMonsterController::Update()
{
	if (m_pMonster)
		UpdateControleState();

	if (m_pCrtBehaviour)
		m_pCrtBehaviour->During();
}

void CMonsterController::OnDestroy()
{
	Safe_Release(m_pMonster);

	for (TRAVERSAL_ITER(m_mBehaviourList, it))
		Safe_Release((*it).second);

	m_mBehaviourList.clear();
}

CMonster* CMonsterController::Get_Monster()
{
	return m_pMonster;
}

void CMonsterController::Set_Monster(CMonster* _monster)
{
	m_pMonster = _monster;

	if (m_pMonster)
		m_pMonster->AddRef();
}

void CMonsterController::UpdateControleState()
{
	CTransform* transform = m_pMonster->Get_Transform();
	const vector3 myPosition = transform->Get_Position();
	const vector3 myAngle = transform->Get_EulerAngles();

	CPlayer* player = CGameManager::GetInstance().Get_Player();
	CTransform* playerTf = player->Get_Transform();
	const vector3 playerPos = playerTf->Get_Position();
	const vector3 playerAnge = playerTf->Get_EulerAngles();
}

const _uint CMonsterController::Get_CrtState() const
{
	return m_iCrtState;
}

void CMonsterController::ChangeState(MonsterState _state)
{
	if (m_bDead)
		return;

	if (m_pCrtBehaviour)
		m_pCrtBehaviour->Exit();

	m_pCrtBehaviour = m_mBehaviourList[_state];
	m_pCrtBehaviour->Enter();

	m_iCrtState = static_cast<MonsterState>(_state);
}

const _bool CMonsterController::IsDamaged() const
{
	return m_bDamaged;
}

void CMonsterController::SetDamaged(const _bool _value)
{
	m_bDamaged = _value;
}

const _bool CMonsterController::IsDead() const
{
	return m_bDead;
}

void CMonsterController::SetDead()
{
	m_bDead = true;
}
