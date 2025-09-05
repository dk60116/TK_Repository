#pragma once
#include "MonsterBehaviour.h"

class CBehaviour_Death : public CMonsterBehaviour
{
	friend class CMonsterController;

private:
	explicit CBehaviour_Death();
	~CBehaviour_Death();

public:
	HRESULT Initialize(class CMonster* _monster) override;

	void Enter(void* _desc) override;
	void During() override;
	void Exit() override;
};

