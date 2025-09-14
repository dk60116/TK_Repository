#pragma once
#include "MonsterBehaviour.h"

class CDragonBHV_Idle final : public CMonsterBehaviour
{
	friend class CBossController;

private:
	explicit CDragonBHV_Idle();
	~CDragonBHV_Idle();

public:
	HRESULT Initialize(class CMonster* _monster) override;

	void Enter(void* _desc) override;
	void During() override;
	void Exit() override;
};

