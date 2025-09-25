#pragma once
#include "MonsterBehaviour.h"

class CDragonBHV_CombatWait final : public CMonsterBehaviour
{
	friend class CBossController;

private:
	explicit CDragonBHV_CombatWait();
	~CDragonBHV_CombatWait();

public:
	HRESULT Initialize(class CMonster* _monster) override;

	void Enter(void* _desc) override;
	void During() override;
	void Exit() override;

	CMonsterController::TurnDir m_eTurnDir, m_ePrevTurnDir;
};

