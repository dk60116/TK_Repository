#pragma once
#include "MonsterBehaviour.h"

class CDragonBHV_Dead final : public CMonsterBehaviour
{
	friend class CBossController;

private:
	explicit CDragonBHV_Dead();
	~CDragonBHV_Dead();

public:
	HRESULT Initialize(class CMonster* _monster) override;

	void Enter(void* _desc) override;
	void During() override;
	void Exit() override;
};

