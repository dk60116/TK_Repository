#include "epch.h"
#include "Material.h"
#include "Shader.h"

CMaterial::CMaterial()
	: m_pShader(nullptr)
	, m_pMatrixBuffer(nullptr)
	, m_pCameraBuffer(nullptr)
	, m_pMaterialBuffer(nullptr)
	, m_vTextureList({})
	, m_vDiffuseColor(ColorValue::white())
{
	m_strName = L"Material";
}

CMaterial::CMaterial(const CMaterial& _other)
	: m_pShader(_other.m_pShader)
	, m_pMatrixBuffer(_other.m_pMatrixBuffer)
	, m_pCameraBuffer(_other.m_pCameraBuffer)
	, m_pMaterialBuffer(_other.m_pMaterialBuffer)
	, m_vTextureList({})
	, m_vDiffuseColor(ColorValue::white())
{
	m_strName = L"Material (Clone)";
}

CMaterial::~CMaterial()
{
	OnDestroy();
}

CMaterial* CMaterial::Create(const wstring _path)
{
	CMaterial* newMaterial = new CMaterial();

	return newMaterial;
}

CMaterial* CMaterial::Clone(const CMaterial& _other)
{
	CMaterial* cloneMaterial = new CMaterial(_other);

	return cloneMaterial;
}

HRESULT CMaterial::Initialize(const wstring& _name, wstring _filePath, void* _desc)
{
	if (FAILED(__super::Initialize(_name, _filePath, _desc)))
		return E_FAIL;

	if (_desc)
	{
		MATERIALDESC* matDesc = reinterpret_cast<MATERIALDESC*>(_desc);
		Set_Shader(matDesc->shaderPointer);
	}

	if (FAILED(Create_ConstantBuffer()))
		return E_FAIL;

	return S_OK;
}

void CMaterial::OnDestroy()
{
	Safe_Release(m_pShader);
	Safe_Release(m_pMatrixBuffer);
	Safe_Release(m_pCameraBuffer);
	Safe_Release(m_pMaterialBuffer);

	for (TRAVERSAL_ITER(m_vTextureList, it))
		Safe_Release(*it);
}

void CMaterial::Bind(const _fmatrix _world, const _cmatrix _view, const _cmatrix _projection, const _uint _boneCount) const
{
	ID3D11DeviceContext* context = CGraphicDevice::GetInstance().Get_Context();

	if (m_pShader)
		m_pShader->Bind();

	Bind_Texture();

	// b0: PerObject
	struct PerObjectCB { _matrix world; };
	PerObjectCB obj{};
	obj.world = XMMatrixTranspose(_world);
	context->UpdateSubresource(m_pMatrixBuffer, 0, nullptr, &obj, 0, 0);
	context->VSSetConstantBuffers(0, 1, &m_pMatrixBuffer);

	// b1: PerCamera
	struct PerCameraCB { _matrix view; _matrix proj; };
	PerCameraCB cam{};
	cam.view = XMMatrixTranspose(_view);
	cam.proj = XMMatrixTranspose(_projection);
	context->UpdateSubresource(m_pCameraBuffer, 0, nullptr, &cam, 0, 0);
	context->VSSetConstantBuffers(1, 1, &m_pCameraBuffer);

	// b2: PerMaterial
	MaterialCB mat = {};
	mat.baseColor = XMFLOAT4
	(
		m_vDiffuseColor.r / 255.f,
		m_vDiffuseColor.g / 255.f,
		m_vDiffuseColor.b / 255.f,
		m_vDiffuseColor.a / 255.f
	);
	mat.useTexture = (!m_vTextureList.empty() && m_vTextureList[0] != nullptr);
	mat.boneCount = _boneCount;
	mat.padding = { 0, 0 };

	context->UpdateSubresource(m_pMaterialBuffer, 0, nullptr, &mat, 0, 0);
	context->VSSetConstantBuffers(2, 1, &m_pMaterialBuffer);
	context->PSSetConstantBuffers(2, 1, &m_pMaterialBuffer);
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

void CMaterial::Set_Shader(CShader* _shader)
{
	if (_shader == m_pShader)
		return;

	Safe_Release(m_pShader);

	if (_shader)
	{
		m_pShader = _shader;
		m_pShader->AddRef();
	}
}

HRESULT CMaterial::Create_ConstantBuffer()
{
	ID3D11Device* device = CGraphicDevice::GetInstance().Get_Device();

	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// 式式 b0 : MatrixCB (VS)
	desc.ByteWidth = sizeof(MatrixCB);
	if (FAILED(device->CreateBuffer(&desc, nullptr, &m_pMatrixBuffer)))
		return E_FAIL;

	// 式 b1 : View/Proj Matrix
	desc.ByteWidth = sizeof(_matrix) * 2;
	if (FAILED(device->CreateBuffer(&desc, nullptr, &m_pCameraBuffer)))
		return E_FAIL;

	// 式式 b2 : MaterialCB (PS)
	desc.ByteWidth = sizeof(MaterialCB);
	if (FAILED(device->CreateBuffer(&desc, nullptr, &m_pMaterialBuffer)))
		return E_FAIL;

	return S_OK;
}

void CMaterial::Bind_Texture() const
{
	ID3D11DeviceContext* context = CGraphicDevice::GetInstance().Get_Context();

	ID3D11ShaderResourceView* texture = nullptr;

	if (!m_vTextureList.empty() && m_vTextureList[0])
		texture = m_vTextureList[0]->Get_SRV();

	if (CEngineString::Contains(m_strName, L"Clone"))
		int a = 0;

	context->PSSetShaderResources(0, 1, &texture);

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

		if (FAILED(CGraphicDevice::GetInstance().Get_Device()->CreateSamplerState(&sampDesc, &gSamplerState)))
		{
			CDebug::LogError(L"Create failed SamplerState in Material");
			return;
		}
	}

	context->PSSetSamplers(0, 1, &gSamplerState);
}
