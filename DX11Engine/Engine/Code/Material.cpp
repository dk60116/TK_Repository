#include "epch.h"
#include "Material.h"
#include "Shader.h"

CMaterial::CMaterial()
	: m_pShader(nullptr)
	, m_pMatrixBuffer(nullptr)
	, m_pCameraBuffer(nullptr)
	, m_pMaterialBuffer(nullptr)
	, m_pLightBuffer(nullptr)
	, m_pCustomBuffer(nullptr)
	, m_vCustomBufferByteList({})
	, m_bUseLight(false)
	, m_vTextureList({})
	, m_mIntValues({})
	, m_mFloatValues({})
	, m_mVector2Values({})
	, m_mVector3Values({})
	, m_mVector4Values({})
	, m_mMatrixValues({})
{
	m_strName = L"Material";
}

CMaterial::CMaterial(const CMaterial& _other)
	: m_pShader(_other.m_pShader)
	, m_pMatrixBuffer(_other.m_pMatrixBuffer)
	, m_pCameraBuffer(_other.m_pCameraBuffer)
	, m_pMaterialBuffer(_other.m_pMaterialBuffer)
	, m_pLightBuffer(_other.m_pLightBuffer)
	, m_pCustomBuffer(_other.m_pCustomBuffer)
	, m_vCustomBufferByteList({})
	, m_bUseLight(_other.m_bUseLight)
	, m_vTextureList({})
	, m_mFloatValues(_other.m_mFloatValues)
	, m_mIntValues(_other.m_mIntValues)
	, m_mVector2Values(_other.m_mVector2Values)
	, m_mVector3Values(_other.m_mVector3Values)
	, m_mVector4Values(_other.m_mVector4Values)
	, m_mMatrixValues(_other.m_mMatrixValues)
{
	m_strName = L"Material (Clone)";

	if (m_pShader)
		m_pShader->AddRef();
	if (m_pMatrixBuffer)
		m_pMatrixBuffer->AddRef();
	if (m_pCameraBuffer)
		m_pCameraBuffer->AddRef();
	if (m_pMaterialBuffer)
		m_pMaterialBuffer->AddRef();
	if (m_pLightBuffer)
		m_pLightBuffer->AddRef();
	if (m_pCustomBuffer)
		m_pCameraBuffer->AddRef();

	BaseInitValues();
}

CMaterial::~CMaterial()
{
	OnDestroy();
}

CMaterial* CMaterial::Create(const wstring _path)
{
	return new CMaterial();
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
		m_bUseLight = matDesc->usingRight;

		for (const auto& [key, value] : matDesc->customFloatValues)
		{
			m_mFloatValues.emplace(key, value);
			const BYTE* p = reinterpret_cast<const BYTE*>(&value);
			m_vCustomBufferByteList.push_back(*p);
		}
		for (const auto& [key, value] : matDesc->customIntValues)
		{
			m_mIntValues.emplace(key, value);
			const BYTE* p = reinterpret_cast<const BYTE*>(&value);
			m_vCustomBufferByteList.push_back(value);
		}
		for (const auto& [key, value] : matDesc->customVector2Values)
		{
			m_mVector2Values.emplace(key, value);
			const BYTE* pX = reinterpret_cast<const BYTE*>(&value.x);
			const BYTE* pY = reinterpret_cast<const BYTE*>(&value.y);
			m_vCustomBufferByteList.insert(m_vCustomBufferByteList.end(), pX, pX + sizeof(_float));
			m_vCustomBufferByteList.insert(m_vCustomBufferByteList.end(), pY, pY + sizeof(_float));
		}
		for (const auto& [key, value] : matDesc->customVector3Values)
		{
			m_mVector3Values.emplace(key, value);
		}
		for (const auto& [key, value] : matDesc->customVector4Values)
		{
			m_mVector4Values.emplace(key, value);
		}
		for (const auto& [key, value] : matDesc->customMatrixValues)
		{
			m_mMatrixValues.emplace(key, value);
		}
	}

	if (FAILED(Create_ConstantBuffer()))
	{
		CDebug::LogError(L"Material - Create_ConstantBuffer Failed: " + m_strResourceName);
		return E_FAIL;
	}

	BaseInitValues();

	return S_OK;
}

void CMaterial::OnDestroy()
{
	Safe_Release(m_pShader);
	Safe_Release(m_pMatrixBuffer);
	Safe_Release(m_pCameraBuffer);
	Safe_Release(m_pMaterialBuffer);
	Safe_Release(m_pLightBuffer);

	m_vCustomBufferByteList.clear();

	for (TRAVERSAL_ITER(m_vTextureList, it))
		Safe_Release(*it);
	m_vTextureList.clear();
}

void CMaterial::BaseInitValues()
{
	{
		m_mFloatValues.emplace(L"Smoothness", 0.f);
	}
	{
		m_mVector4Values.emplace(L"DiffuseColor", ColorValue::white().f4Color());
	}
}

void CMaterial::Bind_Matrix(const _fmatrix _world)
{
	ID3D11DeviceContext* context = CGraphicDevice::GetInstance().Get_Context();

	// b0: PerObject
	MatrixCB matrixCB = {};
	matrixCB.world = XMMatrixTranspose(_world);
	context->UpdateSubresource(m_pMatrixBuffer, 0, nullptr, &matrixCB, 0, 0);
	context->VSSetConstantBuffers(0, 1, &m_pMatrixBuffer);
}

void CMaterial::Bind_Camera(const _float3 _camPos, const _fmatrix _view, const _cmatrix _projection, const _uint _boneCount)
{
	ID3D11DeviceContext* context = CGraphicDevice::GetInstance().Get_Context();

	if (m_pShader)
		m_pShader->Bind();

	Bind_Texture();

	// b1: PerCamera
	CameraCB camCB = {};
	camCB.camPos = _camPos;
	camCB.view = XMMatrixTranspose(_view);
	camCB.proj = XMMatrixTranspose(_projection);
	context->UpdateSubresource(m_pCameraBuffer, 0, nullptr, &camCB, 0, 0);
	context->VSSetConstantBuffers(1, 1, &m_pCameraBuffer);

	// b2: PerMaterial
	MaterialCB mat = {};

	mat.baseColor = XMFLOAT4
	(
		m_mVector4Values.at(L"DiffuseColor").x,
		m_mVector4Values.at(L"DiffuseColor").y,
		m_mVector4Values.at(L"DiffuseColor").z,
		m_mVector4Values.at(L"DiffuseColor").w
	);

	mat.useTexture = (!m_vTextureList.empty() && m_vTextureList[0] != nullptr);
	mat.smoothness = m_mFloatValues.at(L"Smoothness");
	mat.boneCount = _boneCount;
	mat.padding = 0.f;

	context->UpdateSubresource(m_pMaterialBuffer, 0, nullptr, &mat, 0, 0);
	context->VSSetConstantBuffers(2, 1, &m_pMaterialBuffer);
	context->PSSetConstantBuffers(2, 1, &m_pMaterialBuffer);
}

void CMaterial::Bind_Light(_matrix* _lights, const _uint _count)
{
	if (m_pCustomBuffer)
		Bind_CustomValues();

	if (!_lights || !m_bUseLight || !m_pLightBuffer)
		return;

	ID3D11DeviceContext* context = CGraphicDevice::GetInstance().Get_Context();

	LightCB buffer = {};

	// 데이터 복사
	const _uint maxCount = min(_count, 64u);
	memcpy(buffer.lights, _lights, sizeof(_matrix) * maxCount);

	context->UpdateSubresource(m_pLightBuffer, 0, nullptr, &buffer, 0, 0);
	context->PSSetConstantBuffers(4, 1, &m_pLightBuffer);
}

void CMaterial::Bind_CustomValues()
{
	ID3D11DeviceContext* context = CGraphicDevice::GetInstance().Get_Context();

	context->UpdateSubresource(m_pCustomBuffer, 0, nullptr, m_vCustomBufferByteList.data(), 0, 0);
	context->PSSetConstantBuffers(10, 1, &m_pCustomBuffer);
}

const _bool CMaterial::IsUseLight() const
{
	return m_bUseLight;
}

CTexture* CMaterial::Get_Texture(_int _index) const
{
	return m_vTextureList[_index];
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

void CMaterial::Set_FloatValue(const wstring _key, const _float _value)
{
	auto it = m_mFloatValues.find(_key);

	if (it != m_mFloatValues.end())
		m_mFloatValues[_key] = _value;
	else
		CDebug::LogError(L"Material - Set_FloatValue Failed - Key not found: " + _key  + L" - " + m_strResourceName);
}

void CMaterial::Set_IntValue(const wstring _key, const _int _value)
{
	auto it = m_mFloatValues.find(_key);

	if (it != m_mFloatValues.end())
		m_mIntValues[_key] = _value;
	else
		CDebug::LogError(L"Material - Set_IntValue Failed - Key not found: " + _key + L" - " + m_strResourceName);
}

void CMaterial::Set_Vector2Value(const wstring _key, const _float2 _value)
{
	auto it = m_mVector2Values.find(_key);

	if (it != m_mVector2Values.end())
		m_mVector2Values[_key] = _value;
	else
		CDebug::LogError(L"Material - Set_Vector2Value Failed - Key not found: " + _key + L" - " + m_strResourceName);
}

void CMaterial::Set_Vector3Value(const wstring _key, const _float3 _value)
{
	auto it = m_mVector3Values.find(_key);

	if (it != m_mVector3Values.end())
		m_mVector3Values[_key] = _value;
	else
		CDebug::LogError(L"Material - Set_Vector3Value Failed - Key not found: " + _key + L" - " + m_strResourceName);
}

void CMaterial::Set_Vector4Value(const wstring _key, const _float4 _value)
{
	auto it = m_mVector4Values.find(_key);

	if (it != m_mVector4Values.end())
		m_mVector4Values[_key] = _value;
	else
		CDebug::LogError(L"Material - Set_Vector4Value Failed - Key not found: " + _key + L" - " + m_strResourceName);
}

void CMaterial::SetMatrixValue(const wstring _key, const _float4x4 _value)
{
	auto it = m_mMatrixValues.find(_key);

	if (it != m_mMatrixValues.end())
		m_mMatrixValues[_key] = _value;
	else
		CDebug::LogError(L"Material - Set_MatrixValue Failed - Key not found: " + _key + L" - " + m_strResourceName);
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

	// b0 : MatrixCB (VS)
	desc.ByteWidth = sizeof(MatrixCB);
	if (FAILED(device->CreateBuffer(&desc, nullptr, &m_pMatrixBuffer)))
		return E_FAIL;

	// b1 : View/Proj Matrix (VS)
	desc.ByteWidth = sizeof(CameraCB);
	if (FAILED(device->CreateBuffer(&desc, nullptr, &m_pCameraBuffer)))
		return E_FAIL;

	// b2 : MaterialCB (PS)
	desc.ByteWidth = sizeof(MaterialCB);
	if (FAILED(device->CreateBuffer(&desc, nullptr, &m_pMaterialBuffer)))
		return E_FAIL;

	// b4: Light (PS)
	if (m_bUseLight)
	{
		desc.ByteWidth = sizeof(LightCB);

		if (FAILED(device->CreateBuffer(&desc, nullptr, &m_pLightBuffer)))
			return E_FAIL;
	}
	
	// b10: Custom
	if (m_vCustomBufferByteList.size() > 0)
	{
		desc.ByteWidth = static_cast<_uint>(sizeof(m_vCustomBufferByteList));
		auto a = desc.ByteWidth;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		if (FAILED(device->CreateBuffer(&desc, nullptr, &m_pCustomBuffer)))
			return E_FAIL;

		if (!m_pCustomBuffer)
		{
			CDebug::LogError(L"Material - Create_ConstantBuffer Failed - m_pCustomBuffer is null: " + m_strResourceName);
			return E_FAIL;
		}
	}

	return S_OK;
}

void CMaterial::Bind_Texture() const
{
	ID3D11DeviceContext* context = CGraphicDevice::GetInstance().Get_Context();

	ID3D11ShaderResourceView* texture = nullptr;

	if (!m_vTextureList.empty() && m_vTextureList[0])
		texture = m_vTextureList[0]->Get_SRV();

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
