#include "epch.h"
#include "Camera.h"

CCamera::CCamera()
	: m_eCamViewMode()
	, m_vViewMatrix(XMMatrixIdentity())
	, m_vProjMatrix(XMMatrixIdentity())
	, m_vBackgroundColor(ColorValue(49, 77, 121, 255))
	, m_fNear(0.1f)
	, m_fFar(600.f)
	, m_fFieldOfView(60.f)
	, m_fSize(5.f)
{
}

CCamera::~CCamera()
{
}

CCamera* CCamera::Create()
{
	return new CCamera();
}

HRESULT CCamera::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CCamera::Update()
{
	Bind_ViewMatrix();
	Bind_ProjectionMatrix();
}

const CCamera::ViewMode CCamera::Get_ViewMode() const
{
	return m_eCamViewMode;
}

void CCamera::Set_ViewMode(const ViewMode _mode)
{
	m_eCamViewMode = _mode;
}

void CCamera::SetNear(const _float _value)
{
	m_fNear = _value;
}

void CCamera::SetFar(const _float _value)
{
	m_fFar = _value;
}

const ColorValue& CCamera::Get_BackgroundColor() const
{
	return m_vBackgroundColor;
}

void CCamera::Bind_ViewMatrix()
{
	m_vViewMatrix = m_pGameObject->Get_Transfrom()->Get_InverseWorldMatrix();
}

void CCamera::Bind_ProjectionMatrix()
{
	const _float aspect = CDisplay::GetInstance().Get_Aspect();

	switch (m_eCamViewMode)
	{
	case Engine::CCamera::PERSPECTIVE:
	{
		m_vProjMatrix = XMMatrixPerspectiveFovLH
		(
			XMConvertToRadians(m_fFieldOfView),
			aspect,
			m_fNear,
			m_fFar
		);
	}
	break;
	case Engine::CCamera::ORTHOGRAPHIC:
	{
		const _float fHalfHeight = m_fSize * 0.5f;
		const _float fHalfWidth = fHalfHeight * aspect;

		m_vProjMatrix = XMMatrixOrthographicOffCenterLH
		(
			-fHalfWidth, fHalfWidth,
			-fHalfHeight, fHalfHeight,
			m_fNear,
			m_fFar
		);
	}
	break;
	default:
		break;
	}
}