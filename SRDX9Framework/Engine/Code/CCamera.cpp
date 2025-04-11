#include "CCamera.h"
#include "CScreen.h"
#include "CGameObject.h"

CCamera::CCamera()
	: m_eCamViewMode(PERSPECTIVE)
	, m_sParameters({})
	, m_matProjMatrix()
{
}

CCamera::~CCamera()
{
}

void CCamera::Awake()
{
}

void CCamera::Start()
{
}

void CCamera::Update()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void CCamera::FixedUpdate()
{
}

void CCamera::LateUpdate()
{
}

void CCamera::Render()
{
}

void CCamera::OnEnable()
{
}

void CCamera::OnDisable()
{
}

void CCamera::OnDestroy()
{
}

void CCamera::UpdateProjectionMatrix()
{
	switch (m_eCamViewMode)
	{
	case PERSPECTIVE:
		D3DXMatrixPerspectiveFovLH
		(
			&m_matProjMatrix,
			m_sParameters.m_fFovY,
			m_sParameters.m_fAspect,
			m_sParameters.m_fNearZ,
			m_sParameters.m_fFarZ
		);
		break;
	case ORTHOGRAPHIC:
	{
		float width = m_sParameters.m_fFovY * m_sParameters.m_fAspect;
		float height = m_sParameters.m_fFovY;

		D3DXMatrixOrthoLH(
			&m_matProjMatrix,
			width,
			height,
			m_sParameters.m_fNearZ,
			m_sParameters.m_fFarZ
		);
	}
		break;
	default:
		break;
	}
}

void CCamera::UpdateViewMatrix()
{
	CTransform& tf = getTransform();

	vector3 eye = tf.getPosition();
	vector3 fwd = tf.getDirections().forward;
	vector3 at = eye + fwd;
	vector3 up = tf.getDirections().up;

	D3DXVECTOR3 eyePos = eye.vector();
	D3DXVECTOR3 atPos = at.vector();
	D3DXVECTOR3 upVec = up.vector();

	D3DXMatrixLookAtLH(&m_matViewMatrix, &eyePos, &atPos, &upVec);
}

void CCamera::ResetAspectFromResolution()
{
	if (CScreen::GetInstance().getResolution().y > 0)
		m_sParameters.m_fAspect = (float)CScreen::GetInstance().getResolution().x / (float)CScreen::GetInstance().getResolution().y;
}
