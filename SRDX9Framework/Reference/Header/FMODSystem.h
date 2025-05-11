#pragma once

#include "Object.h"

BEGIN(Engine)

class ENGINE_DLL CFMODSystem final
    : public UObject
{
    SINGLETONCLASS(CFMODSystem);

public:
    HRESULT Init();
    void Update();
    virtual void Free();

    FMOD::System* getSystem() { return m_pSystem; }

private:
    FMOD::System* m_pSystem;
};

END

