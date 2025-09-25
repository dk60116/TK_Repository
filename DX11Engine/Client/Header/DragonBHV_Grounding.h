#pragma once
#include "MonsterBehaviour.h"

class CDragonBHV_Grounding final : public CMonsterBehaviour
{
	friend class CBossController;

private:
	explicit CDragonBHV_Grounding();
	~CDragonBHV_Grounding();

public:
	HRESULT Initialize(class CMonster* _monster) override;

	void Enter(void* _desc) override;
	void During() override;
	void Exit() override;
};

