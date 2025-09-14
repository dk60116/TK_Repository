#pragma once
#include "MonsterBehaviour.h"

class CDragonBHV_ShootFireBall final : public CMonsterBehaviour
{
	friend class CBossController;

private:
	explicit CDragonBHV_ShootFireBall();
	~CDragonBHV_ShootFireBall();

public:
	HRESULT Initialize(class CMonster* _monster) override;

	void Enter(void* _desc) override;
	void During() override;
	void Exit() override;

private:
	_bool m_bShooted;
};

