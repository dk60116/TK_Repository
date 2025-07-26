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
		if ((*it)->Get_GameObject()->IsActive() && (*it)->Get_Enable())
			(*it)->Render_WithCamera(this);
	}

	m_vMeshList.clear();
}

void CCamera::RenderUI()
{
	_matrix viewMat = XMMatrixTranslation(50.f, 50.f, 0.f);

	_vector det = {};
	const _matrix inverseMat = XMMatrixInverse(&det, viewMat);

	_float4 camPosF4;
	XMStoreFloat4(&camPosF4, inverseMat.r[3]);

	const _float aspect = CDisplay::GetInstance().Get_Aspect();
	const _float fHalfHeight = 7.2f * 0.5f;
	const _float fHalfWidth = fHalfHeight * aspect;

	const _matrix projMat = XMMatrixOrthographicOffCenterLH
	(
		-fHalfWidth, fHalfWidth,
		-fHalfHeight, fHalfHeight,
		0.f, 1.f
	);

	if (m_vUIList.size() <= 0)
		return;

	for (TRAVERSAL_ITER(m_vUIList, it))
	{
		if ((*it)->Get_GameObject()->IsActive() && (*it)->Get_Enable())
		{
			if (auto img = dynamic_cast<CImage*>(*it))
				img->Bind_UIMaterial();
			(*it)->Bind_Matrix();
			(*it)->Bind_Camera(inverseMat, projMat);
			(*it)->Bind_Mesh();
		}
	}

	m_vUIList.clear();
}

CPhysics::Ray CCamera::ScreenPointToRay(const vector2Int& _pixel, _float _maxDist)
{
	auto res = CDisplay::GetInstance().Get_ScreenResolution();
	_float w = static_cast<float>(res.x);
	_float h = static_cast<float>(res.y);

	_float xNdc = 2.0f * _pixel.x / w - 1.0f;
	_float yNdc = -2.0f * _pixel.y / h + 1.0f;

	_vector ptNear = XMVectorSet(xNdc, yNdc, 0.f, 1.f);
	_vector ptFar = XMVectorSet(xNdc, yNdc, 1.f, 1.f);

	_matrix view = XMLoadFloat4x4(&m_vViewMatrix);
	_matrix proj = XMLoadFloat4x4(&m_vProjMatrix);
	_matrix invVP = XMMatrixInverse(nullptr, view * proj);

	ptNear = XMVector4Transform(ptNear, invVP);
	ptFar = XMVector4Transform(ptFar, invVP);
	ptNear /= XMVectorGetW(ptNear);
	ptFar /= XMVectorGetW(ptFar);

	_float3 origin, dir;
	XMStoreFloat3(&origin, ptNear);

	if (m_eCamViewMode == PERSPECTIVE)
	{
		_vector rayDir = XMVectorSubtract(ptFar, ptNear);
		rayDir = XMVector3Normalize(rayDir);
		XMStoreFloat3(&dir, rayDir);
	}
	else
	{
		XMStoreFloat3(&origin, ptNear);
		dir = Get_Transform()->Get_Directions().forward;
	}

	vector3 resultDir = vector3(dir);

	return CPhysics::Ray{ origin, resultDir.normalized(), _maxDist };
}

CPhysics::Ray CCamera::ScreenPointToRay_Editor(const vector2Int& _pixel, _float _maxDist)
{
	auto eo = CEditor::GetInstance().Get_Options();

	auto res = CEditor::GetInstance().Get_ScreenResolution();
	_float w = static_cast<_float>(res.x - 40);
	_float h = static_cast<_float>(res.y + 20);

	_float xNdc = 2.0f * _pixel.x / w - 1.0f;
	_float yNdc = -2.0f * _pixel.y / h + 1.0f;

	_vector ptNear = XMVectorSet(xNdc, yNdc, 0.f, 1.f);
	_vector ptFar = XMVectorSet(xNdc, yNdc, 1.f, 1.f);

	_matrix view = XMLoadFloat4x4(&m_vViewMatrix);
	_matrix proj = XMLoadFloat4x4(&m_vProjMatrix);
	_matrix invVP = XMMatrixInverse(nullptr, view * proj);

	ptNear = XMVector4Transform(ptNear, invVP);
	ptFar = XMVector4Transform(ptFar, invVP);
	ptNear /= XMVectorGetW(ptNear);
	ptFar /= XMVectorGetW(ptFar);

	_float3 origin, dir;
	XMStoreFloat3(&origin, ptNear);

	if (m_eCamViewMode == PERSPECTIVE)
	{
		_vector camPos = Get_Transform()->Get_WorldMatrix().r[3];
		_vector rayDir = XMVectorSubtract(ptFar, camPos);

		XMStoreFloat3(&origin, camPos);
		XMStoreFloat3(&dir, rayDir);
	}
	else
	{
		XMStoreFloat3(&origin, ptNear);
		dir = Get_Transform()->Get_Directions().forward;
	}

	vector3 resultDir = vector3(dir);

	CPhysics::Ray result = { origin, resultDir.normalized(), _maxDist };

	return result;
}
