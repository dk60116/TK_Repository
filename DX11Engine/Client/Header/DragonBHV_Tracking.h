#pragma once
#include "MonsterBehaviour.h"

class CDragonBHV_Tracking final : public CMonsterBehaviour
{
	friend class CBossController;

private:
	explicit CDragonBHV_Tracking();
	~CDragonBHV_Tracking();

public:
	HRESULT Initialize(class CMonster* _monster) override;

	void Enter(void* _desc) override;
	void During() override;
	void Exit() override;
};

