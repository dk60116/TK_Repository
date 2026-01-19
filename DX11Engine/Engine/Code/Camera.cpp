#include "epch.h"
#include "Camera.h"

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
	, m_vMeshList({})
	, m_vUIList({})
	, m_mRTDebugDisplays({})
	, m_pRTDebugDS(nullptr)
	, m_pRTDebugRS(nullptr)
	, m_pRTDebugBS(nullptr)
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

HRESULT CCamera::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	// Quad
	CMeshBuffer* quad = CResources::GetInstance().LoadOnGame<CMeshBuffer>(L"Quad (Mesh Buffer)");
	if (!quad)
	{
		CDebug::LogError(L"Not found Quad (Mesh Buffer)");
		return E_FAIL;
	}
	quad->AddRef();

	// Present Material (DeferredPresent.hlsl을 사용하는 머티리얼)
	CMaterial* presentMat = CResources::GetInstance().LoadOnGame<CMaterial>(L"DeferredPresentMaterial (Material)");
	if (!presentMat)
	{
		quad->Release();
		CDebug::LogError(L"Not found DeferredPresentMaterial (Material)");
		return E_FAIL;
	}
	presentMat->AddRef();

	// 4개 디스플레이 등록
	auto PushDisplay = [&](CRenderTarget::RTType type)
		{
			RTDebugDisplay desc = {};
			desc.type = type;
			desc.quad = quad;        desc.quad->AddRef();
			desc.material = presentMat; desc.material->AddRef();

			m_mRTDebugDisplays[type] = desc;
		};

	PushDisplay(CRenderTarget::RTType::Albedo);
	PushDisplay(CRenderTarget::RTType::Normal);
	PushDisplay(CRenderTarget::RTType::Depth);
	PushDisplay(CRenderTarget::RTType::Shading);

	// 로컬 참조 해제 (entry들이 AddRef 했으므로)
	quad->Release();
	presentMat->Release();

	// Debug pipeline states 생성
	ID3D11Device* device = CGraphicDevice::GetInstance().Get_Device();
	if (!device)
		return E_FAIL;

	// DepthStencil: DepthTest OFF
	{
		D3D11_DEPTH_STENCIL_DESC ds = {};
		ds.DepthEnable = FALSE;
		ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		ds.DepthFunc = D3D11_COMPARISON_ALWAYS;
		ds.StencilEnable = FALSE;

		if (FAILED(device->CreateDepthStencilState(&ds, &m_pRTDebugDS)))
			return E_FAIL;
	}

	// Rasterizer: Cull OFF
	{
		D3D11_RASTERIZER_DESC rs = {};
		rs.FillMode = D3D11_FILL_SOLID;
		rs.CullMode = D3D11_CULL_NONE;
		rs.FrontCounterClockwise = FALSE;
		rs.DepthClipEnable = TRUE;

		if (FAILED(device->CreateRasterizerState(&rs, &m_pRTDebugRS)))
			return E_FAIL;
	}

	// Blend: AlphaBlend (선택, 지금은 그냥 덮어쓰기여도 됨)
	{
		D3D11_BLEND_DESC bs = {};
		bs.AlphaToCoverageEnable = FALSE;
		bs.IndependentBlendEnable = FALSE;

		D3D11_RENDER_TARGET_BLEND_DESC rt = {};
		rt.BlendEnable = TRUE;
		rt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		rt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		rt.BlendOp = D3D11_BLEND_OP_ADD;
		rt.SrcBlendAlpha = D3D11_BLEND_ONE;
		rt.DestBlendAlpha = D3D11_BLEND_ZERO;
		rt.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		rt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		bs.RenderTarget[0] = rt;

		if (FAILED(device->CreateBlendState(&bs, &m_pRTDebugBS)))
			return E_FAIL;
	}

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
	for (auto& kv : m_mRTDebugDisplays)
	{
		Safe_Release(kv.second.quad);
		Safe_Release(kv.second.material);
	}

	m_mRTDebugDisplays.clear();

	Safe_Release(m_pRTDebugDS);
	Safe_Release(m_pRTDebugRS);
	Safe_Release(m_pRTDebugBS);
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
		if ((*it)->Get_GameObject()->IsRecursiveActive() && (*it)->Get_Enable())
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
		if ((*it)->Get_GameObject()->IsRecursiveActive() && (*it)->Get_Enable())
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

void CCamera::RenderRTDebugDisplay()
{
	if (m_mRTDebugDisplays.empty())
		return;

	ID3D11DeviceContext* context = CGraphicDevice::GetInstance().Get_Context();
	if (!context)
		return;

	ID3D11RenderTargetView* oldRTV[8] = {};
	ID3D11DepthStencilView* oldDSV = nullptr;
	context->OMGetRenderTargets(8, oldRTV, &oldDSV);

	ID3D11RenderTargetView* backRTV = CGraphicDevice::GetInstance().Get_BackBuffer_RTV();
	ID3D11DepthStencilView* backDSV = CGraphicDevice::GetInstance().Get_DepthStencil_DSV();
	if (!backRTV)
	{
		for (int i = 0; i < 8; ++i) Safe_Release(oldRTV[i]);
		Safe_Release(oldDSV);
		return;
	}

	context->OMSetRenderTargets(1, &backRTV, backDSV);

	UINT oldVpCount = 1;
	D3D11_VIEWPORT oldVp = {};
	context->RSGetViewports(&oldVpCount, &oldVp);

	const auto res = CDisplay::GetInstance().Get_ScreenResolution();
	const float screenW = (float)res.x;
	const float screenH = (float)res.y;
	if (screenW <= 0.f || screenH <= 0.f)
	{
		context->RSSetViewports(1, &oldVp);
		context->OMSetRenderTargets(8, oldRTV, oldDSV);
		for (int i = 0; i < 8; ++i) Safe_Release(oldRTV[i]);
		Safe_Release(oldDSV);
		return;
	}

	float thumbW = screenW * 0.22f;
	float thumbH = screenH * 0.22f;
	thumbW = max(160.f, min(360.f, thumbW));
	thumbH = max(90.f, min(240.f, thumbH));

	const float margin = 10.f;
	const float gap = 8.f;

	const _matrix view = XMMatrixIdentity();
	const _matrix proj = XMMatrixOrthographicOffCenterLH(0.f, screenW, screenH, 0.f, 0.f, 1.f);
	const _float3 camPos = _float3(0.f, 0.f, 0.f);

	const float blendFactor[4] = { 0,0,0,0 };
	context->OMSetDepthStencilState(m_pRTDebugDS, 0);
	context->RSSetState(m_pRTDebugRS);
	context->OMSetBlendState(m_pRTDebugBS, blendFactor, 0xFFFFFFFF);

	CRenderTarget::RTType order[4] =
	{
		CRenderTarget::RTType::Albedo,
		CRenderTarget::RTType::Normal,
		CRenderTarget::RTType::Depth,
		CRenderTarget::RTType::Shading
	};

	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };

	for (int i = 0; i < 4; ++i)
	{
		auto it = m_mRTDebugDisplays.find(order[i]);
		if (it == m_mRTDebugDisplays.end())
			continue;

		RTDebugDisplay& dp = it->second;
		if (!dp.quad || !dp.material)
			continue;

		ID3D11ShaderResourceView* srv = CRenderTargetManager::GetInstance().GetSRV(order[i]);
		if (!srv)
			continue;

		const int col = i % 2;
		const int row = i / 2;

		const float x = margin + col * (thumbW + gap);
		const float y = (screenH - margin - thumbH) - row * (thumbH + gap);

		D3D11_VIEWPORT vp = {};
		vp.TopLeftX = x;
		vp.TopLeftY = y;
		vp.Width = thumbW;
		vp.Height = thumbH;
		vp.MinDepth = 0.f;
		vp.MaxDepth = 1.f;
		context->RSSetViewports(1, &vp);

		const float cx = x + thumbW * 0.5f;
		const float cy = y + thumbH * 0.5f;

		_matrix world =
			XMMatrixScaling(thumbW, thumbH, 1.f) *
			XMMatrixTranslation(cx, cy, 0.f);

		dp.material->Bind_Matrix(world);
		dp.material->Bind_Camera(camPos, view, proj, 0);

		context->PSSetShaderResources(0, 1, &srv);

		dp.quad->Render();

		context->PSSetShaderResources(0, 1, nullSRV);
	}

	context->RSSetViewports(1, &oldVp);

	context->OMSetBlendState(nullptr, blendFactor, 0xFFFFFFFF);
	context->OMSetDepthStencilState(nullptr, 0);
	context->RSSetState(nullptr);

	context->OMSetRenderTargets(8, oldRTV, oldDSV);

	for (int i = 0; i < 8; ++i) Safe_Release(oldRTV[i]);
		Safe_Release(oldDSV);
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
