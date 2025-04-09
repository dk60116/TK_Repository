#pragma once

#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTriCol :
    public CVIBuffer
{
public:
	explicit CTriCol();
	explicit CTriCol(const CTriCol& _rhs);
	virtual ~CTriCol();

public:
	void Awake() override;
	void Render() override;

public:
	HRESULT Ready_Buffer() override;
	void Render_Buffer() override;

public:
	virtual CComponent* Clone();
};

END

