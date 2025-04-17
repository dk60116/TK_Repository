#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

struct MaterialOptions
{
	ColorValue diffuseColor = ColorValue::white();
	ColorValue ambientColor = ColorValue::gray(0.3f);
	ColorValue specularColor = ColorValue::white();
	_float power = 5.f;
};

class ENGINE_DLL CMaterial final
{
public:
	CMaterial();
	~CMaterial();

public:
	void Apply(LPDIRECT3DDEVICE9 _pDevice);
	MaterialOptions& getOptions() { return m_sOptions; }

private:
	D3DMATERIAL9 m_sMaterial;
	MaterialOptions m_sOptions;
};

END
