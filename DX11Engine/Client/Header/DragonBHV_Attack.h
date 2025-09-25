#pragma once
#include "MonsterBehaviour.h"

class CDragonBHV_Attack final : public CMonsterBehaviour
{
	friend class CBossController;

private:
	explicit CDragonBHV_Attack();
	~CDragonBHV_Attack();

public:
	HRESULT Initialize(class CMonster* _monster) override;

	void Enter(void* _desc) override;
	void During() override;
	void Exit() override;
};

