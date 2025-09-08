#include "epch.h"
#include "RenderTarget.h"

CRenderTarget::CRenderTarget()
	: m_strTargetName(L"")
	, m_pRTV(nullptr)
	, m_pSRV(nullptr)
	, m_pTexture2D(nullptr)
	, m_vWorldMatrix()
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

	m_strTargetName = _name;
	m_vClearColor = _clearColor;

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

	const _float w = static_cast<_float>(_size.x);
	const _float h = static_cast<_float>(_size.y);

	_matrix S = XMMatrixScaling(w, h, 1.f);
	_matrix T = XMMatrixTranslation
	(
		_pos.x - vp.Width * 0.5f + w * 0.5f,    
		-(_pos.y - vp.Height * 0.5f + h * 0.5f),
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
		// PerCamera: float3 camPos; float4x4 view; float4x4 proj; float pad;
		struct CamCB { _float3 camPos; _float _pad; _float4x4 view; _float4x4 proj; };
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.ByteWidth = sizeof(CamCB);
		if (FAILED(dev->CreateBuffer(&bd, nullptr, &m_pCBPerCamera)))
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
		if (FAILED(CGraphicDevice::GetInstance().Get_Device()->CreateSamplerState(&sd, &m_pDebugSampler)))
			return E_FAIL;
	}

	return S_OK;
}
#endif

HRESULT CRenderTarget::Render()
{
#ifndef _CLIENT_BUILD
	if (!m_pDefferdShader || !m_pMeshBuffer || !m_pSRV)
		return E_FAIL;

	auto* ctx = CGraphicDevice::GetInstance().Get_Context();
	if (!ctx) 
		return E_FAIL;

	// 셰이더 바인딩(IL/VS/PS)
	m_pDefferdShader->Bind();

	// b0 : world
	_matrix W = XMLoadFloat4x4(&m_vWorldMatrix);
	_matrix WT = XMMatrixTranspose(W);
	ctx->UpdateSubresource(m_pCBPerObject, 0, nullptr, &WT, 0, 0);
	ctx->VSSetConstantBuffers(0, 1, &m_pCBPerObject);

	// b1 : view/proj(직교 투영)
	struct CamCB { _float3 camPos; float _pad; _float4x4 view; _float4x4 proj; };

	_uint numViewports = 1;
	D3D11_VIEWPORT vp{};
	ctx->RSGetViewports(&numViewports, &vp);

	_matrix V = XMMatrixIdentity();
	// 화면 픽셀 좌표계를 그대로 쓰는 직교 투영(-w/2~w/2, -h/2~h/2)
	_matrix P = XMMatrixOrthographicOffCenterLH(-vp.Width * 0.5f, vp.Width * 0.5f, -vp.Height * 0.5f, vp.Height * 0.5f, 0.0f, 1.0f);

	CamCB cam{};
	cam.camPos = _float3(0, 0, 0);
	XMStoreFloat4x4(&cam.view, XMMatrixTranspose(V));
	XMStoreFloat4x4(&cam.proj, XMMatrixTranspose(P));

	ctx->UpdateSubresource(m_pCBPerCamera, 0, nullptr, &cam, 0, 0);
	ctx->VSSetConstantBuffers(1, 1, &m_pCBPerCamera);

	// 텍스처 & 샘플러 (PS t0, s0)
	ctx->PSSetShaderResources(0, 1, &m_pSRV);
	ctx->PSSetSamplers(0, 1, &m_pDebugSampler);

	// 사각형 그리기
	m_pMeshBuffer->Render();

	// SRV 언바인드(다음 패스에서 RTV로 쓰일 가능성 대비)
	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	ctx->PSSetShaderResources(0, 1, nullSRV);

	return S_OK;
#else
	return S_OK;
#endif
}

ID3D11RenderTargetView* CRenderTarget::Get_RTV() const
{
	return m_pRTV;
}

HRESULT CRenderTarget::Bind_Shader()
{
	auto* ctx = CGraphicDevice::GetInstance().Get_Context();
	
	if (!ctx || !m_pSRV) 
		return E_FAIL;

	ctx->PSSetShaderResources(0, 1, &m_pSRV);

	return S_OK;
}

void CRenderTarget::Clear()
{
	auto* ctx = CGraphicDevice::GetInstance().Get_Context();
	
	if (!ctx || !m_pRTV)
		return;

	const auto c = m_vClearColor.f4Color();
	const _float clr[4] = { c.x, c.y, c.z, c.w };

	ctx->ClearRenderTargetView(m_pRTV, clr);
}
