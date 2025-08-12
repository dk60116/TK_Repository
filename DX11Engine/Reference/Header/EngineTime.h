#pragma once

#include "epch.h"
#include "Timer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CTime final
{
	SINGLETONCLASS(CTime);

public:
	static HRESULT Initialize();

public:
	static HRESULT Ready_Time();
	static void Update();
	static void Release();

public:
	static _int Get_FPS();
	static _float Get_DeltaTime();
	static _float Get_ElaspedTime();
	static void SetTimeScale(const _float _value);

private:
	CTimer* m_pTimer;
	float m_fTimeScale;
};

NS_END

