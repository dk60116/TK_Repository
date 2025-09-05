#pragma once
#include "MonsterBehaviour.h"

class CBehaviour_GetHit : public CMonsterBehaviour
{
	friend class CMonsterController;

private:
	explicit CBehaviour_GetHit();
	~CBehaviour_GetHit();

public:
	HRESULT Initialize(class CMonster* _monster) override;

	void Enter(void* _desc) override;
	void During() override;
	void Exit() override;
};

