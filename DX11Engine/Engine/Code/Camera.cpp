// Camera.cpp

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
	, m_pRectBuffer(nullptr)
	, m_vRectMats({})
	, m_pLightingPassMat(nullptr)
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

	if (dynamic_cast<CEditorCamera*>(this))
		return S_OK;

	// Rect
	m_pRectBuffer = CResources::GetInstance().LoadOnGame<CMeshBuffer>(L"Rect (Mesh Buffer)");
	if (!m_pRectBuffer)
	{
		CDebug::LogError("Not found Rect (Mesh Buffer)");
		return E_FAIL;
	}
	m_pRectBuffer->AddRef();

	CMaterial* lightingMat = CResources::GetInstance().LoadOnGame<CMaterial>(L"DeferredShading (Material)");
	if (!lightingMat)
	{
		CDebug::LogError("Not found DeferredShading (Material)");
		return E_FAIL;
	}
	m_pLightingPassMat = lightingMat;
	m_pLightingPassMat->AddRef();

	// Present Material (DeferredPresent.hlsl을 사용하는 머티리얼)
	CMaterial* presentMat = CResources::GetInstance().LoadOnGame<CMaterial>(L"DeferredPresent (Material)");
	if (!presentMat)
	{
		CDebug::LogError("Not found DeferredPresent (Material)");
		return E_FAIL;
	}
	m_vRectMats.push_back(presentMat);
	presentMat->AddRef();

	CMaterial* depthMat = CResources::GetInstance().LoadOnGame<CMaterial>(L"DepthPresent (Material)");
	if (!depthMat)
	{
		CDebug::LogError("Not found DepthPresent (Material)");
		return E_FAIL;
	}
	m_vRectMats.push_back(depthMat);
	depthMat->AddRef();

	// 4개 디스플레이 등록
	auto pushDisplay = [&](CRenderTarget::RTType type, CMaterial* mat)
		{
			RTDebugDisplay desc = {};
			desc.type = type;
			desc.quad = m_pRectBuffer;
			desc.material = mat;

			m_mRTDebugDisplays[type] = desc;
		};

	pushDisplay(CRenderTarget::RTType::Albedo, presentMat);
	pushDisplay(CRenderTarget::RTType::Normal, presentMat);
	pushDisplay(CRenderTarget::RTType::Depth, depthMat);
	pushDisplay(CRenderTarget::RTType::Shading, presentMat);

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

	// Blend: AlphaBlend
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
	if (dynamic_cast<CEditorCamera*>(this))
		return;

	Safe_Release(m_pRectBuffer);

	for (TRAVERSAL_ITER(m_vRectMats, it))
		Safe_Release(*it);

	m_vRectMats.clear();

	Safe_Release(m_pLightingPassMat);

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
	ID3D11DeviceContext* context = CGraphicDevice::GetInstance().Get_Context();
	if (!context || m_mRTDebugDisplays.empty())
		return;

	// 상태 백업
	ID3D11DepthStencilState* prevDS = nullptr; UINT prevStencilRef = 0;
	ID3D11RasterizerState* prevRS = nullptr;
	ID3D11BlendState* prevBS = nullptr; FLOAT prevBlendFactor[4] = {}; UINT prevSampleMask = 0;

	context->OMGetDepthStencilState(&prevDS, &prevStencilRef);
	context->RSGetState(&prevRS);
	context->OMGetBlendState(&prevBS, prevBlendFactor, &prevSampleMask);

	// 디버그 상태 적용
	context->OMSetDepthStencilState(m_pRTDebugDS, 0);
	context->RSSetState(m_pRTDebugRS);
	const FLOAT bf[4] = { 0,0,0,0 };
	context->OMSetBlendState(m_pRTDebugBS, bf, 0xFFFFFFFF);

	// 스크린 해상도
	auto res = CDisplay::GetInstance().Get_ScreenResolution();
	float screenW = (float)res.x;
	float screenH = (float)res.y;

	// 뷰포트(디버그 출력은 백버퍼 전체)
	{
		D3D11_VIEWPORT vp{};
		vp.TopLeftX = 0.f; vp.TopLeftY = 0.f;
		vp.Width = screenW; vp.Height = screenH;
		vp.MinDepth = 0.f; vp.MaxDepth = 1.f;
		context->RSSetViewports(1, &vp);
	}

	// 픽셀 좌표 Ortho (좌상단 원점)
	_matrix view = XMMatrixIdentity();
	_matrix proj = XMMatrixOrthographicOffCenterLH(0.f, screenW, screenH, 0.f, 0.f, 1.f);
	_float3 camPos = { 0.f, 0.f, -1.f };

	// ------------------------------------------------------------------------------------
	// 썸네일 크기/비율: "크기는 유지" + "게임 뷰포트 비율 유지"
	// ------------------------------------------------------------------------------------
	const float margin = 16.f;
	const float gap = 12.f;

	// 게임 뷰포트 기준 aspect (없으면 화면 기준)
	const D3D11_VIEWPORT* gameVP = CGraphicDevice::GetInstance().Get_GameViewport();
	float srcW = gameVP ? gameVP->Width : screenW;
	float srcH = gameVP ? gameVP->Height : screenH;
	float srcAspect = (srcH > 0.f) ? (srcW / srcH) : 1.f;

	// 최대 박스 크기(체감 크기 유지용) + 세로 스택이 화면을 넘지 않도록 제한
	float maxBox = min(200.f, min(screenW, screenH) * 0.30f);

	// 3개를 아래에서 위로 쌓으므로, 높이 제한 반영(필수에 가깝습니다)
	float availH = screenH - margin * 2.f - gap * 2.f;
	maxBox = min(maxBox, availH / 3.f);

	// aspect 유지하며 rectW/rectH 산출 (둘 중 어느 쪽도 maxBox 초과하지 않게)
	float rectW = maxBox;
	float rectH = rectW / srcAspect;
	if (rectH > maxBox)
	{
		rectH = maxBox;
		rectW = rectH * srcAspect;
	}

	// 원하는 3종 (Albedo/Normal/Depth)
	CRenderTarget::RTType types[] =
	{
		CRenderTarget::RTType::Albedo,
		CRenderTarget::RTType::Normal,
		CRenderTarget::RTType::Depth,
		CRenderTarget::RTType::Shading
	};

	const int kCount = (int)(sizeof(types) / sizeof(types[0]));

	for (int i = 0; i < kCount; ++i)
	{
		auto it = m_mRTDebugDisplays.find(types[i]);
		if (it == m_mRTDebugDisplays.end())
			continue;

		RTDebugDisplay& disp = it->second;
		if (!disp.quad || !disp.material)
			continue;

		// 패널 위치(오른쪽 아래에서 위로 쌓기)
		float cx = screenW - margin - rectW * 0.5f;
		float cy = screenH - margin - rectH * 0.5f - i * (rectH + gap);

		// 쿼드 월드(픽셀 단위 스케일 + 위치)
		_matrix world = XMMatrixScaling(rectW, rectH, 1.f) * XMMatrixTranslation(cx, cy, 0.f);

		// 머티리얼/셰이더 바인드 (VS/PS 세팅)
		disp.material->Bind_Matrix(world);
		disp.material->Bind_Camera(camPos, view, proj, 0);

		// 렌더타겟 SRV를 PS 슬롯 0에 직접 바인딩
		ID3D11ShaderResourceView* srv = CRenderTargetManager::GetInstance().GetSRV(types[i]);
		context->PSSetShaderResources(0, 1, &srv);

		// 드로우
		disp.quad->Render();
	}

	// SRV 해제(경고/바인딩 충돌 방지)
	CRenderTargetManager::GetInstance().Unbind_AllSRVs_PS(context);

	// 상태 복원
	context->OMSetDepthStencilState(prevDS, prevStencilRef);
	context->RSSetState(prevRS);
	context->OMSetBlendState(prevBS, prevBlendFactor, prevSampleMask);

	Safe_Release(prevDS);
	Safe_Release(prevRS);
	Safe_Release(prevBS);
}

void CCamera::RenderLightingPass_ToShading(const D3D11_VIEWPORT* vp)
{
	if (!m_pLightingPassMat || !m_pRectBuffer) return;

	ID3D11DeviceContext* ctx = CGraphicDevice::GetInstance().Get_Context();
	if (!ctx) 
		return;

	auto& RTM = CRenderTargetManager::GetInstance();

	ID3D11ShaderResourceView* srvAlbedo = RTM.GetSRV(CRenderTarget::RTType::Albedo);
	ID3D11ShaderResourceView* srvNormal = RTM.GetSRV(CRenderTarget::RTType::Normal);
	ID3D11RenderTargetView* rtvShading = RTM.GetRTV(CRenderTarget::RTType::Shading);

	if (!srvAlbedo || !srvNormal || !rtvShading) return;

	// SRV hazard 제거
	RTM.Unbind_AllSRVs_PS(ctx);

	// Shading 타겟만
	ctx->OMSetRenderTargets(1, &rtvShading, nullptr);

	// vp 우선 사용
	const D3D11_VIEWPORT* useVP = vp ? vp : CGraphicDevice::GetInstance().Get_GameViewport();
	if (useVP) ctx->RSSetViewports(1, useVP);

	// Clear
	const float clear[4] = { 0,0,0,1 };
	ctx->ClearRenderTargetView(rtvShading, clear);

	// 상태(Depth OFF / Cull OFF / Blend OFF)
	ctx->OMSetDepthStencilState(m_pRTDebugDS, 0);
	ctx->RSSetState(m_pRTDebugRS);
	const FLOAT bf[4] = { 0,0,0,0 };
	ctx->OMSetBlendState(nullptr, bf, 0xFFFFFFFF);

	// 풀스크린 quad 행렬
	float W = useVP ? useVP->Width : (float)CDisplay::GetInstance().Get_ScreenResolution().x;
	float H = useVP ? useVP->Height : (float)CDisplay::GetInstance().Get_ScreenResolution().y;

	_matrix view = XMMatrixIdentity();
	_matrix proj = XMMatrixOrthographicOffCenterLH(0.f, W, H, 0.f, 0.f, 1.f);
	_float3 camPos = { 0.f, 0.f, -1.f };

	float cx = W * 0.5f;
	float cy = H * 0.5f;
	_matrix world = XMMatrixScaling(W, H, 1.f) * XMMatrixTranslation(cx, cy, 0.f);

	list<CLight*> lights = CSceneManager::GetInstance().Get_CrtScene()->Get_LightList();
	const _uint lightCount = static_cast<_uint>(lights.size());

	vector<_matrix> vLightInfos = {};

	_uint index = 0;

	for (TRAVERSAL_ITER(lights, it))
	{
		if (!(*it))
			continue;

		_float4x4 lightInfo = (*it)->To_LightInfo();

		if (index == 0)
			lightInfo._44 = static_cast<_float>(lights.size());
		else
			lightInfo._44 = 0.f;

		vLightInfos.push_back(XMLoadFloat4x4(&lightInfo));

		++index;
	}

	m_pLightingPassMat->Bind_Light(vLightInfos.data(), lightCount);
	_float4x4 my;
	XMMATRIX V = XMLoadFloat4x4(&m_vViewMatrix);
	XMMATRIX P = XMLoadFloat4x4(&m_vProjMatrix);
	XMMATRIX invVP = XMMatrixInverse(nullptr, V * P);
	XMStoreFloat4x4(reinterpret_cast<_float4x4*>(&my), XMMatrixTranspose(invVP));

	m_pLightingPassMat->Bind_Matrix(world);
	m_pLightingPassMat->Bind_Camera(camPos, view, proj, 0);

	ID3D11ShaderResourceView* srvDepth = RTM.GetSRV(CRenderTarget::RTType::Depth);
	if (!srvDepth)
		return;

	ID3D11ShaderResourceView* srvs[3] = { srvAlbedo, srvNormal, srvDepth };
	ctx->PSSetShaderResources(0, 3, srvs);

	// Draw
	m_pRectBuffer->Render();

	// 정리
	RTM.Unbind_AllSRVs_PS(ctx);
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
