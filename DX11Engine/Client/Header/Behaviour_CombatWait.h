#pragma once

#include "MonsterBehaviour.h"

class CBehaviour_CombatWait final : public CMonsterBehaviour
{
	friend class CMonsterController;

private:
	CBehaviour_CombatWait();
	~CBehaviour_CombatWait();

public:
	HRESULT Initialize(class CMonster* _monster) override;

	void Enter() override;
	void During() override;
	void Exit() override;
};

