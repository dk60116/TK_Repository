#include "CLight.h"
#include "CTransform.h"

CLight::CLight()
	: m_eType(DIRECTIONALLIGHT)
	, m_sLightInfo({})
	, m_sOptions({})
{
	Init();
}

CLight::~CLight()
{
}

void CLight::Init()
{
	ZeroMemory(&m_sLightInfo, sizeof(D3DLIGHT9));
	m_sLightInfo.Type = (D3DLIGHTTYPE)m_eType;
	m_sLightInfo.Diffuse = m_sOptions.emissionColor.dvColor();
	m_sLightInfo.Ambient = m_sOptions.ambientColor.dvColor();
	m_sLightInfo.Specular = m_sOptions.specularColor.dvColor();
}

void CLight::Apply(LPDIRECT3DDEVICE9 _device, DWORD _index)
{
	vector3 pos = getTransform().getPosition();
	vector3 dir = getTransform().getDirections().forward;

	switch (m_eType)
	{
	case CLight::DIRECTIONALLIGHT:
		m_sLightInfo.Direction = dir.dVector();
		break;
	case CLight::POINTLIGHT:
		m_sLightInfo.Position = pos.dVector();
		m_sLightInfo.Range = m_sOptions.range;
		m_sLightInfo.Falloff = m_sOptions.fallof;
		break;
	case CLight::SPOTLIGHT:
		m_sLightInfo.Position = pos.dVector();
		m_sLightInfo.Direction = dir.dVector();
		m_sLightInfo.Range = m_sOptions.range;
		m_sLightInfo.Falloff = m_sOptions.fallof;
		m_sLightInfo.Theta = m_sOptions.theta;
		m_sLightInfo.Phi = m_sOptions.phi;
		break;
	default:
		break;
	}

	m_sLightInfo.Type = (D3DLIGHTTYPE)m_eType;
	m_sLightInfo.Diffuse = m_sOptions.emissionColor.dvColor();
	m_sLightInfo.Ambient = m_sOptions.ambientColor.dvColor();
	m_sLightInfo.Specular = m_sOptions.specularColor.dvColor();

	_device->SetLight(_index, &m_sLightInfo);
	_device->LightEnable(_index, TRUE);
}