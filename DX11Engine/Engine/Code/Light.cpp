#include "epch.h"
#include "Light.h"

CLight::CLight()
	: m_eType(LightType::Directional)
	, m_fIntensity(1.f)
	, m_fRange(10.f)
	, m_fSpotAngle(45.f)
	, m_fAttenuation(1.f)
	, m_vDiffuseColor(ColorValue::white())
	, m_vSpecularColor(ColorValue::white())
	, m_bCastShadow(true)
	, m_mShadowView(XMMatrixIdentity())
	, m_mShadowProj(XMMatrixIdentity())
	, m_mShadowViewProj(XMMatrixIdentity())
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
	UpdateShadowCameraMatrices();
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

const CLight::LightType CLight::Get_Type() const
{
	return m_eType;
}

void CLight::Set_Type(const LightType _type)
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

void CLight::Set_Range(const _float _value)
{
	m_fRange = _value;
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
	result._34 = CSceneManager::GetInstance().Get_CrtScene()->Get_LightSetting().ambient;

	result._41 = static_cast<float>(m_eType);
	result._42 = m_fAttenuation;
	result._43 = (m_pGameObject->IsActive() && m_bEnable) ? 1.f : 0.f;
	result._44 = 0.f;

	return result;
}

const _matrix& CLight::Get_ShadowView() const
{
	return m_mShadowView;
}

const _matrix& CLight::Get_ShadowProj() const
{
	return m_mShadowProj;
}

const _matrix& CLight::Get_ShadowViewProj() const
{
	return m_mShadowViewProj;
}

_vector CLight::SafeUpFromDir(_vector dir)
{
	_vector up = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_float d = fabsf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(dir), up)));
	if (d > 0.99f)
		up = XMVectorSet(0.f, 0.f, 1.f, 0.f);
	return up;
}

void CLight::UpdateShadowCameraMatrices()
{
	// 1) Shadow 사용 여부
	if (!m_bCastShadow || !m_bEnable || !m_pGameObject || !m_pGameObject->IsActive())
	{
		m_mShadowView = XMMatrixIdentity();
		m_mShadowProj = XMMatrixIdentity();
		m_mShadowViewProj = XMMatrixIdentity();
		return;
	}

	// 2) Scene 옵션
	CScene* scene = CSceneManager::GetInstance().Get_CrtScene();
	const auto& ls = scene->Get_LightSetting();

	const _float nearZ = ls.shadowNF.x;
	const _float farZ = ls.shadowNF.y;

	// 3) Light transform 기반 pos/dir
	const vector3 pos = Get_Transform()->Get_Position();
	const vector3 fwd = Get_Transform()->Get_Directions().forward;

	_vector Lpos = XMVectorSet(pos.x, pos.y, pos.z, 1.f);
	_vector Ldir = XMVectorSet(fwd.x, fwd.y, fwd.z, 0.f);
	Ldir = XMVector3Normalize(Ldir);

	// 4) Directional에서 "무엇을 바라볼지" (초기버전: 씬 카메라 or 원점)
	_vector focus = XMVectorZero();
	if (scene && scene->Get_Camera())
	{
		const vector3 camPos = scene->Get_Camera()->Get_Transform()->Get_Position();
		focus = XMVectorSet(camPos.x, camPos.y, camPos.z, 1.f);
	}

	_vector up = SafeUpFromDir(Ldir);

	if (m_eType == LightType::Directional)
	{
		const float dist = max(m_fRange, 1.f);
		_vector eye = XMVectorSubtract(focus, XMVectorScale(Ldir, dist));

		m_mShadowView = XMMatrixLookAtLH(eye, focus, up);

		const float halfSize = max(m_fRange, 1.f);
		const float w = halfSize * 2.f;
		const float h = halfSize * 2.f;

		m_mShadowProj = XMMatrixOrthographicLH(w, h, nearZ, farZ);
	}
	else if (m_eType == LightType::spot)
	{
		_vector eye = Lpos;
		_vector at = XMVectorAdd(Lpos, Ldir);

		m_mShadowView = XMMatrixLookAtLH(eye, at, up);

		float fov = XMConvertToRadians(m_fSpotAngle);
		fov = clamp(fov, XMConvertToRadians(1.f), XMConvertToRadians(179.f));

		m_mShadowProj = XMMatrixPerspectiveFovLH(fov, 1.f, nearZ, farZ);
	}
	else
	{
		m_mShadowView = XMMatrixIdentity();
		m_mShadowProj = XMMatrixIdentity();
	}

	m_mShadowViewProj = XMMatrixMultiply(m_mShadowView, m_mShadowProj);
}
