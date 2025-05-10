#include "CCamera.h"
#include "CScreen.h"
#include "GameObject.h"

CCamera::CCamera()
	: m_eCamViewMode(PERSPECTIVE)
	, m_sParameters({})
	, m_matProjMatrix()
	, m_fNear(0.1f)
	, m_fFar(600.f)
	, m_fFieldOfView(60.f)
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

void CCamera::UpdateEditor()
{
	__super::UpdateEditor();

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void CCamera::Update()
{
	__super::Update();

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
	__super::OnDestroy();
}

CCamera* CCamera::Create()
{
	return new CCamera();
}

void CCamera::UpdateProjectionMatrix()
{
	switch (m_eCamViewMode)
	{
	case PERSPECTIVE:
		D3DXMatrixPerspectiveFovLH
		(
			&m_matProjMatrix,
			D3DXToRadian(m_fFieldOfView),
			m_sParameters.ascpect,
			m_fNear,
			m_fFar
		);
	break;
	case ORTHOGRAPHIC:
	{
		float width = D3DXToRadian(m_fFieldOfView) * m_sParameters.ascpect;
		float height = D3DXToRadian(m_fFieldOfView);

		D3DXMatrixOrthoLH
		(
			&m_matProjMatrix,
			width,
			height,
			m_fNear,
			m_fFar
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

void CCamera::ViewProjextion()
{
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matViewMatrix);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProjMatrix);
}

void CCamera::ResetAspectFromResolution(const vector2Int _resolution)
{
	m_sParameters.ascpect = (float)_resolution.x / _resolution.y;

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}
