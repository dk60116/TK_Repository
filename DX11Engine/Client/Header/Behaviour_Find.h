#pragma once

#include "MonsterBehaviour.h"

class CBehaviour_Find final : public CMonsterBehaviour
{
	friend class CMonsterController;

private:
	explicit CBehaviour_Find();
	~CBehaviour_Find();

public:
	HRESULT Initialize(class CMonster* _monster) override;

	void Enter(void* _desc) override;
	void During() override;
	void Exit() override;
};

