#pragma once

#include "UI.h"

NS_BEGIN(Engine)

class ENGINE_DLL CCanvas final : public CUI
{
    friend class CGameObject;

public:
    enum class RenderMode { ScreenSpace_Overlay, ScreenSpace_Camera, WorldSpace };

private:
    explicit CCanvas();
    ~CCanvas();

private:
    static CCanvas* Create();

public:
    HRESULT Initialize() override;
    void Update() override;
};

NS_END
