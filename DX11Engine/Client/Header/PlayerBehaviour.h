#pragma once
#include "cpch.h"

class CPlayerBehaviour abstract : public UObject
{
	friend class CPlayerController;

protected:
	explicit CPlayerBehaviour();
	~CPlayerBehaviour();

public:
	virtual HRESULT Initialize(class CPlayer* _player);

	virtual void Enter(void* _desc = nullptr);
	virtual void During();
	virtual void Exit();

	void OnDestroy();

protected:
	wstring m_strBehaviourName;
	CPlayer* m_pPlayer;

protected:
	_ubyte m_iWeight;
	_float m_fPassedTime;
	_bool m_bAnyTime;
};