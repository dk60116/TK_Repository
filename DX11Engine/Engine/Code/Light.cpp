#include "epch.h"
#include "Light.h"

CLight::CLight()
	: m_eType(Type::Directional)
	, m_fIntensity(1.f)
	, m_fRange(10.f)
	, m_vDiffuseColor(ColorValue::white())
	, m_vSpecularColor(ColorValue::white())
{
}

CLight::~CLight()
{
}

CLight* CLight::Create()
{
	return new CLight();
}

HRESULT CLight::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CLight::Update()
{
}

void CLight::Render_Editor()
{
}

void CLight::Render()
{
}

void CLight::OnDestroy()
{
}

const CLight::Type CLight::Get_Type() const
{
	return m_eType;
}

void CLight::Set_Type(const Type _type)
{
	m_eType = _type;
}

const _float CLight::Get_Intencity() const
{
	return m_fIntensity;
}

void CLight::Set_Intencity(const _float _value)
{
	m_fIntensity = _value;
}

void CLight::Set_Color(const ColorValue _color)
{
	m_vDiffuseColor = _color;
}

const LightInfo CLight::To_LightInfo()
{
	LightInfo result = {};

	const vector3 _pos = Get_Transform()->Get_Position();
	const vector3 _dir = Get_Transform()->Get_Directions().forward;
	result.position = _float3(_pos.x, _pos.y, _pos.z);
	result.intensity = m_fIntensity;
	result.direction = _float3(_dir.x, _dir.y, _dir.z);
	result.color = m_vDiffuseColor.f3Color();
	result.type = static_cast<_uint>(m_eType);
	result.range = m_fRange;

	return result;
}
