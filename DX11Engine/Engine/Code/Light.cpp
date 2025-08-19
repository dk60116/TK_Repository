#include "epch.h"
#include "Light.h"

CLight::CLight()
	: m_eType(Type::Directional)
	, m_fIntensity(1.f)
	, m_fRange(10.f)
	, m_fSpotAngle(45.f)
	, m_fAttenuation(1.f)
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

CComponent* CLight::Clone() const
{
	CLight* clone = new CLight;

	clone->m_eType = this->m_eType;
	clone->m_fIntensity = this->m_fIntensity;
	clone->m_fRange = this->m_fRange;
	clone->m_fSpotAngle = this->m_fSpotAngle;
	clone->m_fAttenuation = this->m_fAttenuation;
	clone->m_vDiffuseColor = this->m_vDiffuseColor;
	clone->m_vSpecularColor = this->m_vSpecularColor;

	return clone;
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

const _float CLight::Get_Intensity() const
{
	return m_fIntensity;
}

void CLight::Set_Intensity(const _float _value)
{
	m_fIntensity = _value;
}

void CLight::Set_Color(const ColorValue _color)
{
	m_vDiffuseColor = _color;
}

const _float4x4 CLight::To_LightInfo()
{
	_float4x4 result = {};

	const vector3 pos = Get_Transform()->Get_Position();
	const vector3 dir = Get_Transform()->Get_Directions().forward;
	const vector3 color = m_vDiffuseColor.f3Color();

	result._11 = pos.x;
	result._12 = pos.y;
	result._13 = pos.z;
	result._14 = m_fRange;

	result._21 = dir.x;
	result._22 = dir.y;
	result._23 = dir.z;
	result._24 = cosf(XMConvertToRadians(m_fSpotAngle * 0.5f));

	result._31 = color.x;
	result._32 = color.y;
	result._33 = color.z;
	result._34 = CSceneManager::Get_CrtScene()->Get_LightSetting().ambient;

	result._41 = static_cast<float>(m_eType);
	result._42 = m_fAttenuation;
	result._43 = (m_pGameObject->ActiveSelf() && m_bEnabled) ? 1.f : 0.f;
	result._44 = 0.f;

	return result;
}
