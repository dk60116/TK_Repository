#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CTimer 
	: public CBase
{
public:
	explicit CTimer();
	virtual ~CTimer();

public:
	int Get_FPS() const { return m_iFPS; }
	_float Get_TimeDelta() const { return m_fTimeDelta; }

public:
	HRESULT Ready_Timer();
	void Update_Timer();

private:
	LARGE_INTEGER m_FrameTime;
	LARGE_INTEGER m_FixTime;
	LARGE_INTEGER m_LastTime;
	LARGE_INTEGER m_CpuTick;

	_float m_fFPSTimer;
	_int m_iFrameCount, m_iFPS;
	_float m_fTimeDelta;

private:
	virtual void Free();
};

END

