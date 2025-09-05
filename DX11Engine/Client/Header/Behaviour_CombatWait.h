#pragma once

#include "MonsterBehaviour.h"

class CBehaviour_CombatWait final : public CMonsterBehaviour
{
	friend class CMonsterController;

private:
	explicit CBehaviour_CombatWait();
	~CBehaviour_CombatWait();

public:
	HRESULT Initialize(class CMonster* _monster) override;

	void Enter(void* _desc) override;
	void During() override;
	void Exit() override;

private:
	CMonsterController::TurnDir m_eTurnDir, m_ePrevTurnDir;
};

