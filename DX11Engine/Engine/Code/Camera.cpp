#include "epch.h"
#include "Camera.h"

CCamera::CCamera()
	: m_eCamViewMode()
	, m_vViewMatrix()
	, m_vProjMatrix()
	, m_vBackgroundColor(ColorValue(49, 77, 121, 255))
	, m_fAspect(1.f)
	, m_fNear(0.1f)
	, m_fFar(600.f)
	, m_fFieldOfView(60.f)
	, m_fSize(5.f)
	, m_vMeshList({})
	, m_vUIList({})
{
	m_strName = L"Camera";
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
	m_fAspect = static_cast<_float>(CDisplay::GetInstance().Get_ScreenResolution().x) / CDisplay::GetInstance().Get_ScreenResolution().y;

	Bind_ViewMatrix();
	Bind_ProjectionMatrix();
}

void CCamera::Render()
{
}

void CCamera::OnDestroy()
{
}

_matrix CCamera::Get_ViewMatrix() const
{
	_matrix result = XMLoadFloat4x4(&m_vViewMatrix);

	return result;
}

_matrix CCamera::Get_ProjectionMatrix() const
{
	_matrix result = XMLoadFloat4x4(&m_vProjMatrix);

	return result;
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

void CCamera::Set_BackgroundColor(const ColorValue& _color)
{
	m_vBackgroundColor = _color;
}

void CCamera::Add_RenderTarget_Mesh(CRenderer* _mesh)
{
	m_vMeshList.push_back(_mesh);
}

void CCamera::Add_RenderTarget_UI(CUI* _ui)
{
	m_vUIList.push_back(_ui);
}

void CCamera::Bind_ViewMatrix()
{
	_matrix inverseWorldMatrix = Get_Transform()->Get_InverseWorldMatrix();

	XMStoreFloat4x4(&m_vViewMatrix, inverseWorldMatrix);
}

void CCamera::Bind_ProjectionMatrix()
{
	switch (m_eCamViewMode)
	{
	case CCamera::PERSPECTIVE:
	{
		_matrix projMat = XMMatrixPerspectiveFovLH
		(
			XMConvertToRadians(m_fFieldOfView),
			m_fAspect,
			m_fNear, m_fFar
		);

		XMStoreFloat4x4(&m_vProjMatrix, projMat);
	}
	break;
	case CCamera::ORTHOGRAPHIC:
	{
		const _float fHalfHeight = m_fSize * 0.5f;
		const _float fHalfWidth = fHalfHeight * m_fAspect;

		_matrix projMat = XMMatrixOrthographicOffCenterLH
		(
			-fHalfWidth, fHalfWidth,
			-fHalfHeight, fHalfHeight,
			m_fNear, m_fFar
		);

		XMStoreFloat4x4(&m_vProjMatrix, projMat);
	}
	break;
	default:
		break;
	}
}

void CCamera::RenderMesh()
{
	for (TRAVERSAL_ITER(m_vMeshList, it))
	{
		(*it)->Render_WithCamera(this);
	}

	m_vMeshList.clear();
}

void CCamera::RenderUI()
{
	_matrix viewMat = XMMatrixTranslation(10.f, 10.f, 0.f);

	_vector det = {};
	const _matrix inverseMat = XMMatrixInverse(&det, viewMat);

	const _float aspect = CDisplay::GetInstance().Get_Aspect();
	const _float fHalfHeight = 7.2f * 0.5f;
	const _float fHalfWidth = fHalfHeight * aspect;

	const _matrix projMat = XMMatrixOrthographicOffCenterLH
	(
		-fHalfWidth, fHalfWidth,
		-fHalfHeight, fHalfHeight,
		0.f, 1.f
	);

	for (TRAVERSAL_ITER(m_vUIList, it))
	{
		(*it)->Bind_Matrix(inverseMat, projMat);
		(*it)->Bind_Mesh();
	}

	m_vUIList.clear();
}