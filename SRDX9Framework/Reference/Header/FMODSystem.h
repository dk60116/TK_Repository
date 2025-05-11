#pragma once

#include "Object.h"
#include "fmod.hpp"

BEGIN(Engine)

class ENGINE_DLL CFMODSystem final
    : public UObject
{
    SINGLETONCLASS(CFMODSystem);

public:
    HRESULT Init();
    void Update();
    void Release();

    FMOD::System* getSystem() { return m_pSystem; }

private:
    FMOD::System* m_pSystem;
};

END

