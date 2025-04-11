#pragma once

#include "pch.h"

class CAudioManager
{
	SingletonClass(CAudioManager);

public:
	void Init();
	void Update();
	void Release();

public:
	FMOD::System* getSystem() { return m_pSystem; }

private:
	FMOD::System* m_pSystem;
};

