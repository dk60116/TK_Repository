#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class CRectCol final
    : public CVIBuffer
{
    friend class CSpriteRenderer;

private:
    explicit CRectCol();
    explicit CRectCol(const CRectCol& _rhs);
    ~CRectCol();

public:
    HRESULT Ready_Buffer(LPDIRECT3DDEVICE9 _device) override;
    void UpdateColor() override;

private:
    HRESULT ReadyRect(LPDIRECT3DDEVICE9 _device);
};

END

