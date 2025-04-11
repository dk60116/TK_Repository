#pragma once

#include "pch.h"

class CTime
{
	SingletonClass(CTime);

public:
	void Update();
	void Render(HDC hDC);
	void Release();

public:
	const double& getDeltaTime() { return m_dDeltaTime; }
	const long double& getGameTime() { return GetTickCount64() / 1000.l; }

private:
	LARGE_INTEGER m_llFrequency;
	LARGE_INTEGER m_llOldCount;

	double m_dDeltaTime;
};