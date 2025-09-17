#pragma once
#include "MonsterBehaviour.h"

class CDragonBHV_RotationFly final : public CMonsterBehaviour
{
	friend class CBossController;

private:
	explicit CDragonBHV_RotationFly();
	~CDragonBHV_RotationFly();

public:
	HRESULT Initialize(class CMonster* _monster) override;

	void Enter(void* _desc) override;
	void During() override;
	void Exit() override;
};

