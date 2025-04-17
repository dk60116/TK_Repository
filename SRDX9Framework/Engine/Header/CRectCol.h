#pragma once

#include "CVIBuffer.h"

BEGIN(Engine)

class CRectCol final
    : public CVIBuffer
{
public:
    explicit CRectCol();
    explicit CRectCol(const CRectCol& _rhs);
    ~CRectCol();

public:
    HRESULT Ready_Buffer() override;
    void UpdateColor() override;

public:
    virtual CComponent* Clone();

private:
    HRESULT ReadyRect();
};

END

