#pragma once
#include "MonsterBehaviour.h"

class CDragonBHV_Landing final : public CMonsterBehaviour
{
	friend class CBossController;

private:
	explicit CDragonBHV_Landing();
	~CDragonBHV_Landing();

public:
	HRESULT Initialize(class CBossMonster* _boss) override;

	void Enter(void* _desc) override;
	void During() override;
	void Exit() override;

private:
	_bool m_bThreadingRise;
	vector3 m_vTargetPos;
};

