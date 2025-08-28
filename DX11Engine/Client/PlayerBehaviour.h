#pragma once
#include "cpch.h"

class CPlayerBehaviour : public UObject
{
	friend class CPlayerController;

protected:
	explicit CPlayerBehaviour();
	~CPlayerBehaviour();

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

