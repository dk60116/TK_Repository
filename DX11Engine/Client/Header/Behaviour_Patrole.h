#pragma once

#include "MonsterBehaviour.h"
#include "Monster.h"

class CBehaviour_Patrole final : public CMonsterBehaviour
{
	friend class CMonsterController;

private:
	explicit CBehaviour_Patrole();
	~CBehaviour_Patrole();

public:
	HRESULT Initialize(class CMonster* _monster) override;

	void Enter(void* _desc) override;
	void During() override;
	void Exit() override;

private:
	_float m_fChangeRandomTime;
	_bool m_bLookAround;
};

