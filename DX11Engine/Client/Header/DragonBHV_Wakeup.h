#pragma once
#include "MonsterBehaviour.h"

class CDragonBHV_Wakeup : public CMonsterBehaviour
{
	friend class CBossController;

private:
	explicit CDragonBHV_Wakeup();
	~CDragonBHV_Wakeup();

public:
	HRESULT Initialize(class CBossMonster* _boss) override;

	void Enter(void* _desc) override;
	void During() override;
	void Exit() override;

private:
	_bool m_bCompleteThreat, m_bCompleteTakeFly;
};

