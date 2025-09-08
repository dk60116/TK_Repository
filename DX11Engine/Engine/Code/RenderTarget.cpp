#include "epch.h"
#include "RenderTarget.h"

CRenderTarget::CRenderTarget()
	: m_strTargetName(L"")
	, m_pRTV(nullptr)
	, m_pSRV(nullptr)
	, m_pTexture2D(nullptr)
	, m_pCBPerMaterial(nullptr)
	, m_sViewPort({})
	, m_vWorldMatrix()
	, m_pCBPerObject(nullptr)
	, m_pCBPerCamera(nullptr)
	, m_pMeshBuffer(nullptr)
	, m_pDefferdShader(nullptr)
	, m_pDebugSampler(nullptr)
{
	m_strName = L"Render Target";
}

CRenderTarget::~CRenderTarget()
{
	OnDestroy();
}

CRenderTarget* CRenderTarget::Create(const wstring& _name, const vector2Int _pos, const vector2Int _size, const DXGI_FORMAT _pixelFormat, const ColorValue _clearC)
{
	CRenderTarget* newTarget = new CRenderTarget();

	if (FAILED(newTarget->Initialize(_name, _pos, _size, _pixelFormat, _clearC)))
	{
		delete newTarget;
		return nullptr;
	}

	return newTarget;
}

HRESULT CRenderTarget::Initialize(const wstring& _name, const vector2Int _pos, const vector2Int _size, const DXGI_FORMAT _pixelFormat, const ColorValue _clearColor)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};

	m_pMeshBuffer = CResources::LoadOnGame<CMeshBuffer>(L"Rect (Mesh Buffer)");
	m_pDefferdShader = CResources::LoadOnGame<CShader>(L"Defferd (Shader)");

	m_pMeshBuffer->AddRef();
	m_pDefferdShader->AddRef();

	textureDesc.Width = _size.x;
	textureDesc.Height = _size.y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = _pixelFormat;

	textureDesc.SampleDesc.Quality = 0;
	textureDesc.SampleDesc.Count = 1;

	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ID3D11Device* device = CGraphicDevice::Get_Device();

	if (FAILED(Ready_Debug(_pos, _size)))
		return E_FAIL;

	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &m_pTexture2D)))
		return E_FAIL;

	if (FAILED(device->CreateRenderTargetView(m_pTexture2D, nullptr, &m_pRTV)))
		return E_FAIL;

	if (FAILED(device->CreateShaderResourceView(m_pTexture2D, nullptr, &m_pSRV)))
		return E_FAIL;

	D3D11_TEXTURE2D_DESC depthDesc{};
	depthDesc.Width = _size.x;
	depthDesc.Height = _size.y;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ID3D11Texture2D* pDepth = nullptr;
	if (FAILED(device->CreateTexture2D(&depthDesc, nullptr, &pDepth)))
		return E_FAIL;
	if (FAILED(device->CreateDepthStencilView(pDepth, nullptr, &m_pDSV)))
	{
		Safe_Release(pDepth); 
		return E_FAIL; 
	}
	Safe_Release(pDepth);

	m_strTargetName = _name;
	m_vClearColor = _clearColor;

	m_sViewPort.TopLeftX = static_cast<_float>(_pos.x);
	m_sViewPort.TopLeftY = static_cast<_float>(_pos.y);
	m_sViewPort.Width = static_cast<_float>(_size.x);
	m_sViewPort.Height = static_cast<_float>(_size.y);
	m_sViewPort.MinDepth = 0.0f;
	m_sViewPort.MaxDepth = 1.0f;

	return S_OK;
}

void CRenderTarget::OnDestroy()
{
#ifndef _CLIENT_BUILD
	Safe_Release(m_pCBPerObject);
	Safe_Release(m_pCBPerCamera);
	Safe_Release(m_pDebugSampler);

	Safe_Release(m_pMeshBuffer);
	Safe_Release(m_pDefferdShader);
#endif

	Safe_Release(m_pSRV);
	Safe_Release(m_pRTV);
	Safe_Release(m_pTexture2D);
	Safe_Release(m_pCBPerMaterial);
}

const wstring& CRenderTarget::Get_RTName()
{
	return m_strTargetName;
}

#ifndef _CLIENT_BUILD
HRESULT CRenderTarget::Ready_Debug(const vector2Int _pos, const vector2Int _size)
{
	_uint numViewports = 1;
	D3D11_VIEWPORT vp{};
	CGraphicDevice::GetInstance().Get_Context()->RSGetViewports(&numViewports, &vp);

	const _float targetWidth = static_cast<float>(_size.x);
	const _float targetHeight = static_cast<float>(_size.y);
	const _float targetAspect = targetWidth / targetHeight;

	const _float vpWidth = vp.Width;
	const _float vpHeight = vp.Height;
	const _float vpAspect = vpWidth / vpHeight;

	_float scaleX = 1.0f;
	_float scaleY = 1.0f;

	if (targetAspect > 1.0f)
	{
		scaleX = targetWidth;
		scaleY = targetWidth / targetAspect;
	}
	else
	{
		scaleY = targetHeight;
		scaleX = targetHeight * targetAspect;
	}

	_matrix S = XMMatrixScaling(scaleX, scaleY, 1.f);
	_matrix T = XMMatrixTranslation(
		_pos.x - vpWidth * 0.5f + scaleX * 0.5f,
		-(_pos.y - vpHeight * 0.5f + scaleY * 0.5f),
		0.f
	);
	XMStoreFloat4x4(&m_vWorldMatrix, XMMatrixMultiply(S, T));

	ID3D11Device* dev = CGraphicDevice::GetInstance().Get_Device();

	if (!m_pCBPerObject)
	{
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.ByteWidth = sizeof(_float4x4);
		if (FAILED(dev->CreateBuffer(&bd, nullptr, &m_pCBPerObject)))
			return E_FAIL;
	}

	if (!m_pCBPerCamera)
	{
		struct CamCB { _float3 camPos; _float _pad; _float4x4 view; _float4x4 proj; };
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.ByteWidth = sizeof(CamCB);
		if (FAILED(dev->CreateBuffer(&bd, nullptr, &m_pCBPerCamera)))
			return E_FAIL;
	}

	if (!m_pCBPerMaterial)
	{
		struct MatCB { XMFLOAT4 baseColor; UINT useTexture; UINT _pad[3]; };
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.ByteWidth = sizeof(MatCB); // 16바이트 배수
		if (FAILED(dev->CreateBuffer(&bd, nullptr, &m_pCBPerMaterial)))
			return E_FAIL;
	}

	if (!m_pDebugSampler)
	{
		D3D11_SAMPLER_DESC sd{};
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sd.AddressU = sd.AddressV = sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		sd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sd.MinLOD = 0;
		sd.MaxLOD = D3D11_FLOAT32_MAX;
		if (FAILED(dev->CreateSamplerState(&sd, &m_pDebugSampler)))
			return E_FAIL;
	}

	return S_OK;
}
#endif

HRESULT CRenderTarget::Render()
{
#ifndef _CLIENT_BUILD
	if (!m_pMeshBuffer || !m_pDefferdShader || !m_pSRV || !m_pCBPerObject || !m_pCBPerCamera || !m_pCBPerMaterial)
		return E_FAIL;

	auto* ctx = CGraphicDevice::GetInstance().Get_Context();
	if (!ctx) return E_FAIL;

	// 0) 현재 OM/VP 상태 저장
	ID3D11RenderTargetView* prevRTV = nullptr;
	ID3D11DepthStencilView* prevDSV = nullptr;
	ctx->OMGetRenderTargets(1, &prevRTV, &prevDSV);

	UINT numVP = 1;
	D3D11_VIEWPORT vp{};
	ctx->RSGetViewports(&numVP, &vp);

	// 1) 프리뷰는 깊이 불필요 → DSV = nullptr 로 바인딩 (컬러 타겟은 유지: 보통 백버퍼)
	ctx->OMSetRenderTargets(1, &prevRTV, nullptr);

	// 2) 직교 투영 구성 (현재 뷰포트 크기 기준)
	_matrix V = XMMatrixIdentity();
	_matrix P = XMMatrixOrthographicOffCenterLH(
		-vp.Width * 0.5f, vp.Width * 0.5f,
		-vp.Height * 0.5f, vp.Height * 0.5f,
		0.0f, 1.0f);

	// 3) 상수버퍼 업데이트 (b0: world, b1: cam(view/proj), b2: material)
	_matrix W = XMLoadFloat4x4(&m_vWorldMatrix);
	_matrix WT = XMMatrixTranspose(W);
	ctx->UpdateSubresource(m_pCBPerObject, 0, nullptr, &WT, 0, 0);

	struct CamCB { XMFLOAT3 camPos; float _pad; XMFLOAT4X4 view; XMFLOAT4X4 proj; };
	CamCB cam{};
	cam.camPos = XMFLOAT3(0, 0, 0);
	XMStoreFloat4x4(&cam.view, XMMatrixTranspose(V));
	XMStoreFloat4x4(&cam.proj, XMMatrixTranspose(P));
	ctx->UpdateSubresource(m_pCBPerCamera, 0, nullptr, &cam, 0, 0);

	struct MatCB { XMFLOAT4 baseColor; UINT useTexture; UINT _pad[3]; };
	MatCB mat{};
	mat.baseColor = XMFLOAT4(1, 1, 1, 1);
	mat.useTexture = 1; // ★ 반드시 1로! 텍스처를 샘플하도록
	ctx->UpdateSubresource(m_pCBPerMaterial, 0, nullptr, &mat, 0, 0);

	// 4) 셰이더 / CB / SRV 바인딩
	m_pDefferdShader->Bind(); // VSMain/PSMain(프리뷰용) 바인딩되도록

	ctx->VSSetConstantBuffers(0, 1, &m_pCBPerObject);
	ctx->VSSetConstantBuffers(1, 1, &m_pCBPerCamera);
	ctx->PSSetConstantBuffers(2, 1, &m_pCBPerMaterial);

	ctx->PSSetShaderResources(0, 1, &m_pSRV);
	ctx->PSSetSamplers(0, 1, &m_pDebugSampler);

	// 5) 직사각형 드로우
	m_pMeshBuffer->Render();

	// 6) SRV 언바인드 (이후 이 텍스처를 RTV로 쓸 때 충돌 방지)
	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	ctx->PSSetShaderResources(0, 1, nullSRV);

	// 7) 상태 복원
	ctx->OMSetRenderTargets(1, &prevRTV, prevDSV);
	Safe_Release(prevRTV);
	Safe_Release(prevDSV);

	return S_OK;
#else
	return S_OK;
#endif
}

ID3D11RenderTargetView* CRenderTarget::Get_RTV() const
{
	return m_pRTV;
}

ID3D11DepthStencilView* CRenderTarget::Get_DSV() const
{
	return m_pDSV;
}

const D3D11_VIEWPORT& CRenderTarget::Get_VP()
{
	return m_sViewPort;
}

HRESULT CRenderTarget::Bind_Shader()
{
#ifndef _CLIENT_BUILD
	// 필수 리소스 점검
	if (!m_pMeshBuffer || !m_pDefferdShader || !m_pSRV || !m_pCBPerObject || !m_pCBPerCamera)
		return E_FAIL;

	auto* ctx = CGraphicDevice::GetInstance().Get_Context();
	if (!ctx) 
		return E_FAIL;

	// ── 0) 기존 OM 타깃 보관
	ID3D11RenderTargetView* prevRTV = nullptr;
	ID3D11DepthStencilView* prevDSV = nullptr;
	ctx->OMGetRenderTargets(1, &prevRTV, &prevDSV);

	// ── 1) 프리뷰는 깊이 테스트가 필요없으므로 DSV = nullptr 로 바인딩
	//      (RTV는 현재 바인딩된 것(보통 백버퍼) 유지)
	ctx->OMSetRenderTargets(1, &prevRTV, nullptr);

	// ── 2) 현재 뷰포트로 직교 투영 구성
	UINT numVP = 1;
	D3D11_VIEWPORT vp{};
	ctx->RSGetViewports(&numVP, &vp);

	_matrix V = XMMatrixIdentity();
	_matrix P = XMMatrixOrthographicOffCenterLH(
		-vp.Width * 0.5f, vp.Width * 0.5f,
		-vp.Height * 0.5f, vp.Height * 0.5f,
		0.0f, 1.0f
	);

	// ── 3) 상수버퍼 갱신 (b0: world, b1: view/proj)
	_matrix W = XMLoadFloat4x4(&m_vWorldMatrix);
	_matrix WT = XMMatrixTranspose(W);
	ctx->UpdateSubresource(m_pCBPerObject, 0, nullptr, &WT, 0, 0);

	struct CamCB { _float3 camPos; _float _pad; _float4x4 view; _float4x4 proj; };
	CamCB cam{};
	cam.camPos = _float3(0, 0, 0);
	XMStoreFloat4x4(&cam.view, XMMatrixTranspose(V));
	XMStoreFloat4x4(&cam.proj, XMMatrixTranspose(P));
	ctx->UpdateSubresource(m_pCBPerCamera, 0, nullptr, &cam, 0, 0);

	// ── 4) 셰이더/상수버퍼/텍스처 바인딩
	m_pDefferdShader->Bind(); // (프리뷰용 VS/PS가 바인딩되도록)
	ctx->VSSetConstantBuffers(0, 1, &m_pCBPerObject);
	ctx->VSSetConstantBuffers(1, 1, &m_pCBPerCamera);

	ctx->PSSetShaderResources(0, 1, &m_pSRV);
	ctx->PSSetSamplers(0, 1, &m_pDebugSampler);

	// ── 5) 프리뷰 사각형 드로우
	m_pMeshBuffer->Render();

	// ── 6) SRV 언바인드(다음 프레임 RTV로 쓸 때 충돌 방지)
	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	ctx->PSSetShaderResources(0, 1, nullSRV);

	// ── 7) OM 타깃 복원
	ctx->OMSetRenderTargets(1, &prevRTV, prevDSV);
	Safe_Release(prevRTV);
	Safe_Release(prevDSV);

	return S_OK;
#else
	return S_OK;
#endif
}

void CRenderTarget::Clear()
{
	auto* ctx = CGraphicDevice::GetInstance().Get_Context();
	if (!ctx) 
		return;

	if (m_pRTV)
	{
		const auto c = m_vClearColor.f4Color();
		const _float clr[4] = { c.x, c.y, c.z, c.w };
		ctx->ClearRenderTargetView(m_pRTV, clr);
	}
	if (m_pDSV)
	{
		ctx->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}
