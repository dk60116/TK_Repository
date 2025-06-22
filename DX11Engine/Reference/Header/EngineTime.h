#pragma once

#include "epch.h"
#include "Timer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CTime final
{
	SINGLETONCLASS(CTime);

public:
	HRESULT Initialize();

public:
	HRESULT Ready_Time();
	void Update();
	void Release();

public:
	_int Get_FPS();
	_float Get_DeltaTime();
	void SetTimeScale(const float _value);

private:
	CTimer* m_pTimer;
	float m_fTimeScale;
};

NS_END

