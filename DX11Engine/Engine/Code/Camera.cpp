#include "epch.h"
#include "Camera.h"
#include "ShadowMap.h"

const ColorValue CCamera::s_vDefaultCameraColor = ColorValue(49, 77, 121, 255);

CCamera::CCamera()
	: m_eCamViewMode()
	, m_vViewMatrix()
	, m_vProjMatrix()
	, m_vBackgroundColor(s_vDefaultCameraColor)
	, m_fAspect(1.f)
	, m_fNear(0.1f)
	, m_fFar(600.f)
	, m_fFieldOfView(60.f)
	, m_fSize(5.f)
	, m_vMeshList_Lit({})
	, m_vMeshList_NoneCull({})
	, m_vUIList({})
	, m_vMeshList_Blend({})
	, m_pShadowMap(nullptr)
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

CComponent* CCamera::Clone() const
{
	CCamera* clone = new CCamera();

	clone->m_eCamViewMode = this->m_eCamViewMode;
	clone->m_vBackgroundColor = s_vDefaultCameraColor;
	clone->m_fAspect = this->m_fAspect;
	clone->m_fNear = this->m_fNear;
	clone->m_fFar = this->m_fFar;
	clone->m_fFieldOfView = this->m_fFieldOfView;
	clone->m_fSize = this->m_fSize;

	return clone;
}

HRESULT CCamera::Initialize(void* _desc)
{
	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	if (FAILED(InitShadowMap()))
		return E_FAIL;

	return S_OK;
}

void CCamera::Update()
{
	m_fAspect = static_cast<_float>(CDisplay::Get_ScreenResolution().x) / CDisplay::Get_ScreenResolution().y;

	Bind_ViewMatrix();
	Bind_ProjectionMatrix();
}

void CCamera::Render()
{
}

void CCamera::OnDestroy()
{
	Safe_Release(m_pShadowMap);
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
	m_vMeshList_Lit.push_back(_mesh);
}

void CCamera::Add_RenderTarget_NoneCullMesh(CRenderer* _mesh)
{
	m_vMeshList_NoneCull.push_back(_mesh);
}

void CCamera::Add_RenderTarget_BlendMesh(CRenderer* _mesh)
{
	m_vMeshList_Blend.push_back(_mesh);
}

void CCamera::Add_RenderTarget_UI(CUI* _ui)
{
	m_vUIList.push_back(_ui);
}

HRESULT CCamera::InitShadowMap()
{
	m_pShadowMap = CShadowMap::Create(2048);

	return m_pShadowMap ? S_OK : E_FAIL;
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

void CCamera::Bind_RenderTarget()
{
	ID3D11RenderTargetView* oldRTV[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = {};
	ID3D11DepthStencilView* oldDSV = nullptr;
	m_pContext->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, oldRTV, &oldDSV);

	_uint oldNumVP = 0;
	m_pContext->RSGetViewports(&oldNumVP, nullptr);
	vector<D3D11_VIEWPORT> oldVPs(oldNumVP);
	
	if (oldNumVP)
		m_pContext->RSGetViewports(&oldNumVP, oldVPs.data());
	
	auto* dfRT = CDisplay::Get_RenderTarget(L"Diffuse");
	auto* nmRT = CDisplay::Get_RenderTarget(L"Normal");
	auto* dpRT = CDisplay::Get_RenderTarget(L"Depth");
	auto* sdRT = CDisplay::Get_RenderTarget(L"Shading");
	auto* spRT = CDisplay::Get_RenderTarget(L"Specular");
	auto* cbRT = CDisplay::Get_RenderTarget(L"Combine");

	ID3D11RenderTargetView* rtvs[] =
	{
		dfRT->Get_RTV(), // SV_TARGET0
		nmRT->Get_RTV(), // SV_TARGET1
		dpRT->Get_RTV(),
	};
	
	ID3D11DepthStencilView* dsv = dfRT->Get_DSV(); 

	ID3D11ShaderResourceView* nullSRV[16] = {};
	m_pContext->PSSetShaderResources(0, 16, nullSRV);
	m_pContext->VSSetShaderResources(0, 16, nullSRV);
	m_pContext->GSSetShaderResources(0, 16, nullSRV);

	m_pContext->OMSetRenderTargets(_countof(rtvs), rtvs, dsv);

	D3D11_VIEWPORT vp = dfRT->Get_VP();
	m_pContext->RSSetViewports(1, &vp);

	dfRT->Clear();
	nmRT->Clear();
	dpRT->Clear();
	sdRT->Clear();
	spRT->Clear();
	cbRT->Clear();

	for (TRAVERSAL_ITER(m_vMeshList_Lit, it))
	{
		if ((*it)->Get_GameObject()->IsRecursiveActive() && (*it)->Get_Enabled())
			(*it)->Render_WithCamera(this);
	}

	for (TRAVERSAL_ITER(m_vMeshList_NoneCull, it))
	{
		if ((*it)->Get_GameObject()->IsRecursiveActive() && (*it)->Get_Enabled())
			(*it)->Render_WithCamera(this);
	}

	const _float blendFactor[4] = { 1.f, 1.f, 1.f, 1.f };
	m_pContext->OMSetBlendState(CSceneManager::Get_CrtScene()->Get_BlendingState(), blendFactor, 0xFFFFFFFF);

	for (TRAVERSAL_ITER(m_vMeshList_Blend, it))
	{
		if ((*it)->Get_GameObject()->IsRecursiveActive() && (*it)->Get_Enabled())
			(*it)->Render_WithCamera(this);
	}

	m_pContext->OMSetBlendState(CSceneManager::Get_CrtScene()->Get_NoneBlendingState(), blendFactor, 0xFFFFFFFF);

	auto sdRTV = sdRT->Get_RTV();
	auto sdVP = sdRT->Get_VP();

	m_pContext->OMSetRenderTargets(1, &sdRTV, nullptr);
	m_pContext->RSSetViewports(1, &sdVP);

	auto nmSRV = nmRT->Get_SRV();
	m_pContext->PSSetShaderResources(2, 1, &nmSRV);

	sdRT->Bind_Light();
	sdRT->Bind_Shader();
	sdRT->Bind_Rect();

	auto cbRTV = cbRT->Get_RTV();
	auto cbVP = cbRT->Get_VP();

	m_pContext->OMSetRenderTargets(1, &cbRTV, nullptr);
	m_pContext->RSSetViewports(1, &cbVP);

	cbRT->Bind_Shader();

	auto dfSRV = dfRT->Get_SRV();
	auto sdSRV = sdRT->Get_SRV();
	m_pContext->PSSetShaderResources(1, 1, &dfSRV);
	m_pContext->PSSetShaderResources(4, 1, &sdSRV);

	_uint oldCount = D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;

	while (oldCount > 0 && !oldRTV[oldCount - 1]) 
		--oldCount;

	m_pContext->OMSetRenderTargets(oldCount, oldRTV, oldDSV);
	
	if (oldNumVP)
		m_pContext->RSSetViewports(oldNumVP, oldVPs.data());

	for (_uint i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
	{
		if (oldRTV[i])
			oldRTV[i]->Release();
	}
	if (oldDSV) 
		oldDSV->Release();
}

void CCamera::RenderMesh()
{
	//Bind_RenderTarget();

	//CDisplay::RenderTargetRender(L"Combine");
	//CDisplay::RenderTargetRender(L"Diffuse");
	//CDisplay::RenderTargetRender(L"Normal");
	//CDisplay::RenderTargetRender(L"Depth");
	//CDisplay::RenderTargetRender(L"Shading");

	//RenderShadowPass();

	m_pContext->RSSetState(CSceneManager::Get_CrtScene()->Get_NoneBlendingResterState());

	for (TRAVERSAL_ITER(m_vMeshList_Lit, it))
	{
		if ((*it)->Get_GameObject()->IsRecursiveActive() && (*it)->Get_Enabled())
			(*it)->Render_WithCamera(this);
	}

	m_pContext->RSSetState(CSceneManager::Get_CrtScene()->Get_NoneBlendingNoneCullResterState());

	for (TRAVERSAL_ITER(m_vMeshList_NoneCull, it))
	{
		if ((*it)->Get_GameObject()->IsRecursiveActive() && (*it)->Get_Enabled())
			(*it)->Render_WithCamera(this);
	}

	const _float blendFactor[4] = { 1.f, 1.f, 1.f, 1.f };
	m_pContext->RSSetState(CSceneManager::Get_CrtScene()->Get_BlendingResterState());
	m_pContext->OMSetBlendState(CSceneManager::Get_CrtScene()->Get_BlendingState(), blendFactor, 0xFFFFFFFF);
	m_pContext->OMSetDepthStencilState(CSceneManager::Get_CrtScene()->Get_TransparentStencillState(), 0);

	vector<CRenderer*> sorted(m_vMeshList_Blend.begin(), m_vMeshList_Blend.end());
	const vector3 camPos = Get_Transform()->Get_Position();

	sort(sorted.begin(), sorted.end(), [&](CRenderer* a, CRenderer* b)
		{
		const _float da = (a->Get_Transform()->Get_Position() - camPos).lengthSq();
		const _float db = (b->Get_Transform()->Get_Position() - camPos).lengthSq();
		return da > db;
		});

	for (auto* r : sorted) 
	{
		if (r->Get_GameObject()->IsRecursiveActive() && r->Get_Enabled())
			r->Render_WithCamera(this);
	}

	m_pContext->RSSetState(CSceneManager::Get_CrtScene()->Get_NoneBlendingResterState());
	m_pContext->OMSetBlendState(CSceneManager::Get_CrtScene()->Get_NoneBlendingState(), blendFactor, 0xFFFFFFFF);
	m_pContext->OMSetDepthStencilState(CSceneManager::Get_CrtScene()->Get_MeshStencillState(), 0);
	
	m_vMeshList_Lit.clear();
	m_vMeshList_NoneCull.clear();
	m_vMeshList_Blend.clear();
}

void CCamera::RenderShadowPass()
{
	if (!m_pShadowMap) 
		return;
}

void CCamera::RenderUI()
{
	_matrix viewMat = XMMatrixTranslation(50.f, 50.f, 0.f);

	_vector det = {};
	const _matrix inverseMat = XMMatrixInverse(&det, viewMat);

	_float4 camPosF4;
	XMStoreFloat4(&camPosF4, inverseMat.r[3]);

	const _float aspect = CDisplay::Get_Aspect();
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
		if ((*it)->Get_GameObject()->IsRecursiveActive() && (*it)->Get_Enabled())
		{
			if (CImage* img = dynamic_cast<CImage*>(*it))
			{
				img->Bind_UIMaterial();
				(*it)->Bind_Matrix();
				(*it)->Bind_Camera(inverseMat, projMat);
				(*it)->Bind_Mesh();
			}
			else if (CText* txt = dynamic_cast<CText*>(*it))
			{
				txt->RenderText();
			}
		}
	}

	m_vUIList.clear();
}

CPhysics::Ray CCamera::ScreenPointToRay(const vector2Int& _pixel, _float _maxDist)
{
	auto res = CDisplay::Get_ScreenResolution();
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
	auto eo = CEditor::Get_Options();

	auto res = CEditor::Get_ScreenResolution();
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
