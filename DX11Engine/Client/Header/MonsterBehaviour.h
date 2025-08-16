#pragma once

#include "Object.h"
#include "Monster.h"
#include "MonsterController.h"

class CMonsterBehaviour abstract : public UObject
{
	friend class CMonsterController;

protected:
	explicit CMonsterBehaviour();
	~CMonsterBehaviour();

public:
	virtual HRESULT Initialize(class CMonster* _monster);

	virtual void Enter();
	virtual void During();
	virtual void Exit();

	void OnDestroy();

protected:
	wstring m_strBehaviourName;
	CMonster* m_pMonster;

protected:
	_float m_fPassedTime;

	_bool m_bAnyTime;
};

