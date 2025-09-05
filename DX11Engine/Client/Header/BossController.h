#pragma once
#include "Component.h"

class CBossController abstract : public CComponent
{
public:
	enum class TurnDir { None, Left, Right };

protected:
	explicit CBossController();
	~CBossController();

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

public:
	class CBossMonster* Get_Monster();
	void Set_Monster(CBossMonster* _monster);

public:
	void Change_State(const _uint _state, void* _desc = nullptr, const _bool _forceEnter = false);
	const _bool IsDamaged() const;
	void SetDamaged(const _bool _value);
	const _bool IsDead() const;
	void SetDead();

protected:
	template<typename T>
	void AddBehaviour(const _uint _state);

private:
	void UpdateControleState();

private:
	CBossMonster* m_pMonster;
	
	_uint m_iCrtState;
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
