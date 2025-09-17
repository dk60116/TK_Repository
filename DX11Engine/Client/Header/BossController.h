#pragma once
#include "MonsterController.h"

class CBossController abstract : public CMonsterController
{
public:
	enum class TurnDir { None, Left, Right };

protected:
	explicit CBossController();
	~CBossController();

public:
	HRESULT Initialize(void* _desc) override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

public:
	class CBossMonster* Get_Monster();
	void Set_Monster(CBossMonster* _monster);

public:
	void ChangeState(const _uint _state, void* _desc = nullptr, const _bool _forceEnter = false);
	void ForceChangeState(const _uint _state, void* _desc = nullptr);

protected:
	template<typename T>
	void AddBehaviour(const _uint _state);

private:
	void UpdateControleState();

private:
	CBossMonster* m_pMonster;
	
	map<_uint, class CMonsterBehaviour*> m_mBehaviourList;
	CMonsterBehaviour* m_pCrtBehaviour;

	unordered_map<_uint, _bool> m_mStateMap, m_mPrevStateMap;

	_bool m_bDamaged, m_bDead;
};

template<typename T>
inline void CBossController::AddBehaviour(const _uint _state)
{
	T* b = new T();
	b->Initialize(m_pMonster);
	b->AddRef();
	m_mBehaviourList.emplace(_state, b);
}
