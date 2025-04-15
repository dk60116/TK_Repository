#pragma once

#include "CComponent.h"

struct LightOptions
{
	ColorValue emissionColor = ColorValue::white();
	ColorValue ambientColor = ColorValue::gray(0.3f);
	ColorValue specularColor = ColorValue::gray(0.4f);

	_bool renderShadow = true;
	_float shadowStrength = 1.f;

	_float range = 10.f;
	_float fallof = 1.f;
	_float theta = 0.5f;
	_float phi = 1.f;
};

class ENGINE_DLL CLight
	: public CComponent
{
public:
	enum LightType { DIRECTIONALLIGHT = 3, POINTLIGHT = 2, SPOTLIGHT = 1 };

public:
	CLight();
	~CLight();

public:
	void Init();
	void Apply(LPDIRECT3DDEVICE9 _device, DWORD _index);
	D3DLIGHT9& getInfo() { return m_sLightInfo; };

public:
	LightType& getType() { return m_eType; }
	void SetType(const LightType _type) { m_eType = _type; }
	LightOptions& getOptions() { return m_sOptions; };

private:
	D3DLIGHT9 m_sLightInfo;
	LightType m_eType;
	LightOptions m_sOptions;
};