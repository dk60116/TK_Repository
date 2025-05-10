#pragma once

#include "Object.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CMeshRenderer;
class CSpriteRenderer;

struct MaterialOptions
{
	ColorValue diffuseColor = ColorValue::white();
	ColorValue ambientColor = ColorValue::gray(0.3f);
	ColorValue specularColor = ColorValue::white();
	_float power = 5.f;
};

class ENGINE_DLL CMaterial final
	: public UObject
{
	friend class CMeshRenderer;
	friend class CSpriteRenderer;

private:
	CMaterial();
	~CMaterial();

public:
	void Apply(LPDIRECT3DDEVICE9 _pDevice);
	MaterialOptions& getOptions() { return m_sOptions; }
	void Destroy();

private:
	D3DMATERIAL9 m_sMaterial;
	MaterialOptions m_sOptions;
};

END
