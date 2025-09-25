#pragma once

#include "EngineDefine.h"

NS_BEGIN(Engine)

class ENGINE_DLL CFMODSystem final
{
    SINGLETONCLASS(CFMODSystem);

public:
    static HRESULT Initialize();
    static void Update();
    static void Release();

public:
    FMOD::System* GetSystem() const;

private:
    FMOD::System* m_pSystem;
};

NS_END

