#pragma once
#include "Base.h"
#include "CTimer.h"

BEGIN(Engine)

class ENGINE_DLL CTimeMgr
{
	SingletonClass(CTimeMgr);

public:
	_int Get_FPS();
	_float Get_TimeDelta();
	void SetTimeScale(const float _value) { m_fTimeScale = _value; }

public:
	HRESULT Ready_Timer();
	void Update_Timer();
	void Release();

private:
	CTimer* m_pTimer;
	float m_fTimeScale;
};

END