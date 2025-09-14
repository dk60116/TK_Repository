#pragma once

#include "Object.h"
#include "Monster.h"
#include "BossMonster.h"
#include "MonsterController.h"

class CMonsterBehaviour abstract : public UObject
{
	friend class CMonsterController;
	friend class CBossController;

protected:
	explicit CMonsterBehaviour();
	~CMonsterBehaviour();

public:
	virtual HRESULT Initialize(class CMonster* _monster);

	virtual void Enter(void* _desc = nullptr);
	virtual void During();
	virtual void Exit();

	void OnDestroy();

protected:
	wstring m_strBehaviourName;
	CMonster* m_pMonster;

protected:
	_ubyte m_iWeight;
	_float m_fPassedTime;
	_bool m_bAnyTime;
};

