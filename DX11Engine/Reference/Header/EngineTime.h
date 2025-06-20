#pragma once

#include "epch.h"
#include "Object.h"
#include "Timer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CTime final : public UObject
{
	SINGLETONCLASS(CTime);

public:
	HRESULT Initialize() override;

public:
	HRESULT Ready_Time();
	void Update();
	void Destroy();

public:
	_int Get_FPS();
	_float Get_DeltaTime();
	void SetTimeScale(const float _value);

private:
	CTimer* m_pTimer;
	float m_fTimeScale;
};

NS_END

