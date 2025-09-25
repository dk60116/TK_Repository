#pragma once
#include "MonsterBehaviour.h"

class CDragonBHV_GetHit final : public CMonsterBehaviour
{
	friend class CBossController;

private:
	explicit CDragonBHV_GetHit();
	~CDragonBHV_GetHit();

public:
	HRESULT Initialize(class CMonster* _monster) override;

	void Enter(void* _desc) override;
	void During() override;
	void Exit() override;

private:
	_uint m_iPrevState;
};

