#pragma once

#include "UObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CTimer 
	: public UObject
{
public:
	explicit CTimer();
	virtual ~CTimer();

public:
	const int Get_FPS() const { return m_iFPS; }
	const _float Get_TimeDelta() const { return m_fTimeDelta; }

public:
	HRESULT Ready_Timer();
	void Update();

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

private:
	_bool m_bFirstUpdate;
};

END

