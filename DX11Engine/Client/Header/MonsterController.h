#pragma once

#include "Component.h"

class CMonsterController : public CComponent
{
public:
	enum MonsterState { Idle, Patrole, Find, Tracking, Combat, CombatWait, GetHit, Death };
	enum class TurnDir { None, Left, Right };

protected:
	explicit CMonsterController();
	~CMonsterController();

public:
	static CMonsterController* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

public:
	class CMonster* Get_Monster();
	void Set_Monster(CMonster* _monster);

public:
	virtual const _uint Get_CrtState() const;
	void ChangeState(const MonsterState _state);
	const _bool IsDamaged() const;
	void SetDamaged(const _bool _value);
	const _bool IsDead() const;
	void SetDead();

private:
	void UpdateControleState();

protected:
	_uint m_iCrtState;
	CMonster* m_pMonster;

	map<_uint, class CMonsterBehaviour*> m_mBehaviourList;
	CMonsterBehaviour* m_pCrtBehaviour;

	unordered_map<_uint, _bool> m_mStateMap, m_mPrevStateMap;
	
	_bool m_bDamaged, m_bDead;
};

