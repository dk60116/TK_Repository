#include "epch.h"
#include "Material.h"

CMaterial::CMaterial()
	: m_pDevice(nullptr)
	, m_pContext(nullptr)
	, m_pVertexShader(nullptr)
	, m_pPixelShader(nullptr)
	, m_pInputLayout(nullptr)
	, m_pMatrixBuffer(nullptr)
	, m_pCameraBuffer(nullptr)
	, m_pMaterialBuffer(nullptr)
	, m_vTextureList({})
	, m_vDiffuseColor(ColorValue::white())
{
	m_strName = L"Material";
}

CMaterial::~CMaterial()
{
	OnDestroy();
}

CMaterial* CMaterial::Create(const wstring _path)
{
	CMaterial* newMaterial = new CMaterial();

	if (FAILED(newMaterial->Initialize(_path)))
		return nullptr;

	return newMaterial;
}

HRESULT CMaterial::Initialize(const wstring _path)
{
	m_pDevice = CGraphicDevice::GetInstance().Get_Device();
	m_pContext = CGraphicDevice::GetInstance().Get_Context();

	m_pDevice->AddRef();
	m_pContext->AddRef();

	wstring path = _path.empty() ? L"../EngineResource/Shader/UnlitColor.hlsl" : _path;

	if (FAILED(Load_Shader(path)))
		return E_FAIL;

	if (FAILED(Create_ConstantBuffer()))
		return E_FAIL;

	return S_OK;
}

void CMaterial::OnDestroy()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pMatrixBuffer);
	Safe_Release(m_pCameraBuffer);
	Safe_Release(m_pMaterialBuffer);

	for (TRAVERSAL_ITER(m_vTextureList, it))
	{
		Safe_Release(*it);
	}
}

void CMaterial::Bind(const _fmatrix _world, const _cmatrix _view, const _cmatrix _projection)
{
	Bind_Shader();
	Bind_Texture();

	m_pContext->IASetInputLayout(m_pInputLayout);
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// b0: PerObject
	struct PerObjectCB { _matrix world; };
	PerObjectCB obj{};
	obj.world = XMMatrixTranspose(_world);
	m_pContext->UpdateSubresource(m_pMatrixBuffer, 0, nullptr, &obj, 0, 0);
	m_pContext->VSSetConstantBuffers(0, 1, &m_pMatrixBuffer);

	// b1: PerCamera
	struct PerCameraCB { _matrix view; _matrix proj; };
	PerCameraCB cam{};
	cam.view = XMMatrixTranspose(_view);
	cam.proj = XMMatrixTranspose(_projection);
	m_pContext->UpdateSubresource(m_pCameraBuffer, 0, nullptr, &cam, 0, 0);
	m_pContext->VSSetConstantBuffers(1, 1, &m_pCameraBuffer);
}

CTexture* CMaterial::Get_Texture(_int _index) const
{
	return m_vTextureList[_index];
}

void CMaterial::Set_DiffuseColor(const ColorValue& _color)
{
	m_vDiffuseColor = _color;
}

void CMaterial::Set_Texture(CTexture* _texture, _int _index)
{
	if (_index < 0)
		return;

	while (m_vTextureList.size() <= _index)
		m_vTextureList.push_back(nullptr);

	Safe_Release(m_vTextureList[_index]);
	m_vTextureList[_index] = _texture;

	if (_texture)
		_texture->AddRef();
}

HRESULT CMaterial::Load_Shader(const wstring& _path)
{
	HRESULT hr = S_OK;

	ComPtr<ID3DBlob> vsBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;

	hr = D3DCompileFromFile(_path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VSMain", "vs_5_0", 0, 0, &vsBlob, &errorBlob);
	
	if (FAILED(hr))
	{
		if (errorBlob)
			CDebug::LogError((char*)errorBlob->GetBufferPointer());
		return hr;
	}

	hr = m_pDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_pVertexShader);
	if (FAILED(hr)) 
		return hr;

	// Pixel Shader 컴파일
	ComPtr<ID3DBlob> psBlob = nullptr;

	hr = D3DCompileFromFile(_path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PSMain", "ps_5_0", 0, 0, &psBlob, &errorBlob);

	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		}
		return hr;
	}

	hr = m_pDevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_pPixelShader);
	
	if (FAILED(hr)) 
		return hr;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",       0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",     0, DXGI_FORMAT_R32G32_FLOAT,       0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT,  0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hr = m_pDevice->CreateInputLayout(layout, _countof(layout),
		vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_pInputLayout);

	return hr;
}

HRESULT CMaterial::Create_ConstantBuffer()
{
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// ── b0 : MatrixCB (VS)
	desc.ByteWidth = sizeof(MatrixCB);
	if (FAILED(m_pDevice->CreateBuffer(&desc, nullptr, &m_pMatrixBuffer)))
		return E_FAIL;

	// ─ b1 : View/Proj Matrix
	desc.ByteWidth = sizeof(_matrix) * 2;
	if (FAILED(m_pDevice->CreateBuffer(&desc, nullptr, &m_pCameraBuffer)))
		return E_FAIL;

	// ── b2 : MaterialCB (PS)
	desc.ByteWidth = sizeof(MaterialCB);
	if (FAILED(m_pDevice->CreateBuffer(&desc, nullptr, &m_pMaterialBuffer)))
		return E_FAIL;

	return S_OK;
}

void CMaterial::Bind_Shader()
{
	// ── b1 : 머티리얼
	MaterialCB mat = {};

	mat.baseColor =
	{
		m_vDiffuseColor.r / 255.f,
		m_vDiffuseColor.g / 255.f,
		m_vDiffuseColor.b / 255.f,
		m_vDiffuseColor.a / 255.f
	};
	
	mat.useTexture = (!m_vTextureList.empty() && m_vTextureList[0] != nullptr);

	m_pContext->UpdateSubresource(m_pMaterialBuffer, 0, nullptr, &mat, 0, 0);
	m_pContext->PSSetConstantBuffers(2, 1, &m_pMaterialBuffer);
}

void CMaterial::Bind_Texture()
{
	ID3D11ShaderResourceView* texture = nullptr;

	if (!m_vTextureList.empty() && m_vTextureList[0])
		texture = m_vTextureList[0]->Get_SRV();

	// ─ Shader 및 InputLayout 설정
	if (m_pVertexShader && m_pPixelShader && m_pInputLayout)
	{
		m_pContext->IASetInputLayout(m_pInputLayout);
		m_pContext->VSSetShader(m_pVertexShader, nullptr, 0);
		m_pContext->PSSetShader(m_pPixelShader, nullptr, 0);
	}

	// ─ ShaderResourceView 바인딩 (t0)
	ID3D11ShaderResourceView* srv = texture;
	m_pContext->PSSetShaderResources(0, 1, &srv);

	// ─ SamplerState 바인딩 (s0)
	static ID3D11SamplerState* gSamplerState = nullptr;

	if (!gSamplerState)
	{
		D3D11_SAMPLER_DESC sampDesc = {};
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		if (FAILED(m_pDevice->CreateSamplerState(&sampDesc, &gSamplerState)))
		{
			CDebug::LogError(L"Creaet failed Sample Material");
			return;
		}
	}

	m_pContext->PSSetSamplers(0, 1, &gSamplerState);
}
