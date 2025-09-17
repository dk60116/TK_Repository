#pragma once
#include "MonsterBehaviour.h"

class CDragonBHV_SpreadFire final : public CMonsterBehaviour
{
	friend class CBossController;

private:
	explicit CDragonBHV_SpreadFire();
	~CDragonBHV_SpreadFire();

public:
	HRESULT Initialize(class CMonster* _monster) override;

	void Enter(void* _desc) override;
	void During() override;
	void Exit() override;
};

