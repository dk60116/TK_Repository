#include "CCamera.h"
#include "CScreen.h"
#include "CGameObject.h"

CCamera::CCamera()
	: m_eCamViewMode(PERSPECTIVE)
	, m_sParameters({})
	, m_matProjMatrix()
{
	m_strName = L"Camera";
}

CCamera::~CCamera()
{
	OnDestroy();
}

void CCamera::Awake()
{
	CComponent::Awake();
}

void CCamera::Start()
{
}

void CCamera::Update()
{
	CComponent::Update();

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
	CComponent::OnDestroy();
}

void CCamera::UpdateProjectionMatrix()
{
	switch (m_eCamViewMode)
	{
	case PERSPECTIVE:
		D3DXMatrixPerspectiveFovLH
		(
			&m_matProjMatrix,
			m_sParameters.fov,
			m_sParameters.ascpect,
			m_sParameters.nearZ,
			m_sParameters.farZ
		);
		break;
	case ORTHOGRAPHIC:
	{
		float width = m_sParameters.fov * m_sParameters.ascpect;
		float height = m_sParameters.fov;

		D3DXMatrixOrthoLH
		(
			&m_matProjMatrix,
			width,
			height,
			m_sParameters.nearZ,
			m_sParameters.farZ
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

	D3DXVECTOR3 eyePos = eye.dVector();
	D3DXVECTOR3 atPos = at.dVector();
	D3DXVECTOR3 upVec = up.dVector();

	D3DXMatrixLookAtLH(&m_matViewMatrix, &eyePos, &atPos, &upVec);
}

void CCamera::ResetAspectFromResolution(const vector2Int& _resolution)
{
	if (CScreen::GetInstance().getGameResolution().y > 0)
		m_sParameters.ascpect = (float)_resolution.x / (float)_resolution.y;
}
