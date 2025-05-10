#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTriCol final
    : public CVIBuffer
{
public:
	explicit CTriCol();
	virtual ~CTriCol();

public:
	HRESULT Ready_Buffer(LPDIRECT3DDEVICE9 _device) override;
	void Render_Buffer(LPDIRECT3DDEVICE9 _device) override;

public:
	virtual CComponent* Clone();
};

END

