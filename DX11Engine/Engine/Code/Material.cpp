#include "epch.h"
#include "Material.h"
#include "Shader.h"

CMaterial::CMaterial()
	: m_pShader(nullptr)
	, m_pMatrixBuffer(nullptr)
	, m_pCameraBuffer(nullptr)
	, m_pMaterialBuffer(nullptr)
	, m_pLightBuffer(nullptr)
	, m_pShadowBuffer(nullptr)
	, m_pCustomBuffer(nullptr)
	, m_vCustomBufferByteList({})
	, m_bUseLight(false)
	, m_bTransparent(false)
	, m_vBaseColor(ColorValue::white().f4Color())
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
	, m_pMatrixBuffer(nullptr)
	, m_pCameraBuffer(nullptr)
	, m_pMaterialBuffer(nullptr)
	, m_pLightBuffer(nullptr)
	, m_pCustomBuffer(nullptr)
	, m_vCustomBufferByteList(_other.m_vCustomBufferByteList)
	, m_bUseLight(_other.m_bUseLight)
	, m_bTransparent(_other.m_bTransparent)
	, m_vTextureList({})
	, m_vBaseColor(ColorValue::white().f4Color())
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

	Create_ConstantBuffer();
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
	return new CMaterial(_other);
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
		m_bTransparent = matDesc->transparent;

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
			m_vCustomBufferByteList.push_back(*p);
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
			const BYTE* pX = reinterpret_cast<const BYTE*>(&value.x);
			const BYTE* pY = reinterpret_cast<const BYTE*>(&value.y);
			const BYTE* pZ = reinterpret_cast<const BYTE*>(&value.z);
			m_vCustomBufferByteList.insert(m_vCustomBufferByteList.end(), pX, pX + sizeof(_float));
			m_vCustomBufferByteList.insert(m_vCustomBufferByteList.end(), pY, pY + sizeof(_float));
			m_vCustomBufferByteList.insert(m_vCustomBufferByteList.end(), pZ, pZ + sizeof(_float));
		}
		for (const auto& [key, value] : matDesc->customVector4Values)
		{
			m_mVector4Values.emplace(key, value);
			const BYTE* pX = reinterpret_cast<const BYTE*>(&value.x);
			const BYTE* pY = reinterpret_cast<const BYTE*>(&value.y);
			const BYTE* pZ = reinterpret_cast<const BYTE*>(&value.z);
			const BYTE* pW = reinterpret_cast<const BYTE*>(&value.w);
			m_vCustomBufferByteList.insert(m_vCustomBufferByteList.end(), pX, pX + sizeof(_float));
			m_vCustomBufferByteList.insert(m_vCustomBufferByteList.end(), pY, pY + sizeof(_float));
			m_vCustomBufferByteList.insert(m_vCustomBufferByteList.end(), pZ, pZ + sizeof(_float));
			m_vCustomBufferByteList.insert(m_vCustomBufferByteList.end(), pW, pW + sizeof(_float));
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

	return S_OK;
}

void CMaterial::OnDestroy()
{
	Safe_Release(m_pShader);
	Safe_Release(m_pMatrixBuffer);
	Safe_Release(m_pCameraBuffer);
	Safe_Release(m_pMaterialBuffer);
	Safe_Release(m_pLightBuffer);
	Safe_Release(m_pShadowBuffer);
	Safe_Release(m_pCustomBuffer);

	m_vCustomBufferByteList.clear();

	for (TRAVERSAL_ITER(m_vTextureList, it))
		Safe_Release(*it);
	m_vTextureList.clear();
}

void CMaterial::Bind_Matrix(const _fmatrix _world)
{
	ID3D11DeviceContext* context = CGraphicDevice::GetInstance().Get_Context();

	// b0: PerObject
	MatrixCB matrixCB = {};
	matrixCB.world = XMMatrixTranspose(_world);
	context->UpdateSubresource(m_pMatrixBuffer, 0, nullptr, &matrixCB, 0, 0);
	context->PSSetConstantBuffers(0, 1, &m_pMatrixBuffer);
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
	_matrix viewInv = XMMatrixInverse(nullptr, _view);
	camCB.viewInv = XMMatrixTranspose(viewInv);
	context->UpdateSubresource(m_pCameraBuffer, 0, nullptr, &camCB, 0, 0);
	context->PSSetConstantBuffers(1, 1, &m_pCameraBuffer);
	context->VSSetConstantBuffers(1, 1, &m_pCameraBuffer);

	// b2: PerMaterial
	MaterialCB mat = {};

	mat.baseColor = m_vBaseColor;
	mat.useTexture = (!m_vTextureList.empty() && m_vTextureList[0] != nullptr);
	mat.boneCount = _boneCount;

	context->UpdateSubresource(m_pMaterialBuffer, 0, nullptr, &mat, 0, 0);
	context->PSSetConstantBuffers(2, 1, &m_pMaterialBuffer);
	context->VSSetConstantBuffers(2, 1, &m_pMaterialBuffer);

	if (m_vCustomBufferByteList.size() > 0)
		Bind_CustomValues();
}

void CMaterial::Bind_Light(_matrix* _lights, const _uint _count)
{
	if (!_lights || !m_bUseLight || !m_pLightBuffer)
		return;

	ID3D11DeviceContext* context = CGraphicDevice::GetInstance().Get_Context();

	LightCB buffer = {};

	// 데이터 복사
	const _uint maxCount = min(_count, MAX_LIGHT);
	memcpy(buffer.lights, _lights, sizeof(_matrix) * maxCount);

	context->UpdateSubresource(m_pLightBuffer, 0, nullptr, &buffer, 0, 0);
	context->VSSetConstantBuffers(4, 1, &m_pLightBuffer);
	context->PSSetConstantBuffers(4, 1, &m_pLightBuffer);
}

void CMaterial::Bind_Shadow(const _fmatrix _world, const _fmatrix _lightVP, const _bool _alphaCutout)
{
	//ID3D11DeviceContext* ctx = CGraphicDevice::GetInstance().Get_Context();

	//// 1) DepthOnly 셰이더 바인딩
	////  - 프로젝트 셰이더 매니저에 맞춰 교체하세요.
	////  - 예: "VS_DepthOnly", "PS_DepthOnly_Null" / "PS_DepthOnly_AlphaTest"
	//{
	//	CShader* vsDepth = CShaderManager::Get()->Get(L"VS_DepthOnly");
	//	CShader* psDepth = nullptr;
	//	if (alphaCutout)
	//		psDepth = CShaderManager::Get()->Get(L"PS_DepthOnly_AlphaTest");
	//	else
	//		psDepth = CShaderManager::Get()->Get(L"PS_DepthOnly_Null"); // 혹은 PS 생략 가능

	//	if (vsDepth) 
	//		vsDepth->BindVS();     // 입력 레이아웃 포함
	//	if (psDepth) 
	//		psDepth->BindPS();
	//	else         
	//		ctx->PSSetShader(nullptr, nullptr, 0); // 완전 depth-only
	//}

	//// 2) b0 : World
	//MatrixCB m = {};
	//m.world = XMMatrixTranspose(_world);
	//ctx->UpdateSubresource(m_pMatrixBuffer, 0, nullptr, &m, 0, 0);
	//ctx->VSSetConstantBuffers(0, 1, &m_pMatrixBuffer); // VS만으로도 충분

	//// 3) b5 : LightVP
	//ShadowCB s = {};
	//s.lightVP = XMMatrixTranspose(_lightVP);
	//ctx->UpdateSubresource(m_pShadowBuffer, 0, nullptr, &s, 0, 0);
	//ctx->VSSetConstantBuffers(5, 1, &m_pShadowBuffer);

	//// 4) 알파 컷아웃이면 베이스 텍스처 t0 바인딩(샘플러는 Bind_Texture에서 만들던 기본 s0 사용)
	//if (_alphaCutout)
	//{
	//	ID3D11ShaderResourceView* base = Get_DiffuseSRV();
	//	ctx->PSSetShaderResources(0, 1, &base);

	//	// 샘플러(선형 래핑) 하나는 기본적으로 만들어 두셨으니 재사용:
	//	// CMaterial::Bind_Texture()를 부르지 않는 이유: 불필요한 CB/텍스처까지 대거 바인딩되기 때문
	//	static ID3D11SamplerState* s0 = nullptr;
	//	
	//	if (!s0)
	//	{
	//		D3D11_SAMPLER_DESC sd = {};
	//		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//		sd.AddressU = sd.AddressV = sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//		sd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	//		sd.MinLOD = 0; sd.MaxLOD = D3D11_FLOAT32_MAX;
	//		CGraphicDevice::GetInstance().Get_Device()->CreateSamplerState(&sd, &s0);
	//	}
	//	ctx->PSSetSamplers(0, 1, &s0);
	//}
}

void CMaterial::Bind_CustomValues()
{
	m_vCustomBufferByteList.clear();

	auto append = [&](const void* src, size_t sz) 
		{
		const BYTE* p = reinterpret_cast<const BYTE*>(src);
		m_vCustomBufferByteList.insert(m_vCustomBufferByteList.end(), p, p + sz);
		};
	auto align16 = [&]() 
		{
		while (m_vCustomBufferByteList.size() % 16 != 0)
			m_vCustomBufferByteList.push_back(0);
		};

	m_vCustomBufferByteList.clear();

	// 순서 중요: HLSL과 일치해야 함
	for (const auto& [key, value] : m_mFloatValues)
	{
		const BYTE* p = reinterpret_cast<const BYTE*>(&value);
		m_vCustomBufferByteList.insert(m_vCustomBufferByteList.end(), p, p + sizeof(_float));
	}
	for (const auto& [key, value] : m_mIntValues)
	{
		const BYTE* p = reinterpret_cast<const BYTE*>(&value);
		m_vCustomBufferByteList.insert(m_vCustomBufferByteList.end(), p, p + sizeof(_int));
	}
	for (const auto& [key, value] : m_mVector2Values)
	{
		const BYTE* px = reinterpret_cast<const BYTE*>(&value.x);
		const BYTE* py = reinterpret_cast<const BYTE*>(&value.y);
		m_vCustomBufferByteList.insert(m_vCustomBufferByteList.end(), px, px + sizeof(_float));
		m_vCustomBufferByteList.insert(m_vCustomBufferByteList.end(), py, py + sizeof(_float));
	}
	for (const auto& [key, value] : m_mVector3Values)
	{
		const BYTE* px = reinterpret_cast<const BYTE*>(&value.x);
		const BYTE* py = reinterpret_cast<const BYTE*>(&value.y);
		const BYTE* pz = reinterpret_cast<const BYTE*>(&value.z);
		m_vCustomBufferByteList.insert(m_vCustomBufferByteList.end(), px, px + sizeof(_float));
		m_vCustomBufferByteList.insert(m_vCustomBufferByteList.end(), py, py + sizeof(_float));
		m_vCustomBufferByteList.insert(m_vCustomBufferByteList.end(), pz, pz + sizeof(_float));
	}
	for (const auto& [key, value] : m_mVector4Values)
	{
		const BYTE* px = reinterpret_cast<const BYTE*>(&value.x);
		const BYTE* py = reinterpret_cast<const BYTE*>(&value.y);
		const BYTE* pz = reinterpret_cast<const BYTE*>(&value.z);
		const BYTE* pw = reinterpret_cast<const BYTE*>(&value.w);
		m_vCustomBufferByteList.insert(m_vCustomBufferByteList.end(), px, px + sizeof(_float));
		m_vCustomBufferByteList.insert(m_vCustomBufferByteList.end(), py, py + sizeof(_float));
		m_vCustomBufferByteList.insert(m_vCustomBufferByteList.end(), pz, pz + sizeof(_float));
		m_vCustomBufferByteList.insert(m_vCustomBufferByteList.end(), pw, pw + sizeof(_float));
	}

	// 정렬 맞추기 (16바이트 단위)
	while (m_vCustomBufferByteList.size() % 16 != 0)
		m_vCustomBufferByteList.push_back(0);

	ID3D11DeviceContext* context = CGraphicDevice::GetInstance().Get_Context();
	context->UpdateSubresource(m_pCustomBuffer, 0, nullptr, m_vCustomBufferByteList.data(), 0, 0);
	context->VSSetConstantBuffers(10, 1, &m_pCustomBuffer);
	context->PSSetConstantBuffers(10, 1, &m_pCustomBuffer);
}

const _bool CMaterial::IsUseLight() const
{
	return m_bUseLight;
}

const _bool CMaterial::IsTransparent() const
{
	return m_bTransparent;
}

CTexture* CMaterial::Get_Texture(_int _index) const
{
	return m_vTextureList[_index];
}

void CMaterial::Set_Texture(CTexture* _texture, _uint _index)
{
	while (m_vTextureList.size() <= _index)
		m_vTextureList.push_back(nullptr);

	if (m_vTextureList.size() < _index + 1)
		m_vTextureList.resize(_index + 1);

	Safe_Release(m_vTextureList[_index]);
	m_vTextureList[_index] = _texture;

	if (_texture)
		_texture->AddRef();
}

void CMaterial::Set_BaseColor(const _float4& _color)
{
	m_vBaseColor = _color;
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
	auto it = m_mIntValues.find(_key);

	if (it != m_mIntValues.end())
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

	// b5: Shadow (LightVP)
	desc.ByteWidth = sizeof(ShadowCB);
	if (FAILED(device->CreateBuffer(&desc, nullptr, &m_pShadowBuffer)))
		return E_FAIL;

	// b10: Custom
	if (m_vCustomBufferByteList.size() > 0)
	{
		_uint byteWidth = (_uint)max<size_t>(16, ((m_vCustomBufferByteList.size() + 15) & ~15));
		byteWidth = (byteWidth + 15) & ~15;
		
		desc.ByteWidth = byteWidth;

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

	const _uint slotCount = static_cast<_uint>(m_vTextureList.size());

	if (slotCount > 0)
	{
		vector<ID3D11ShaderResourceView*> srvs(slotCount, nullptr);
		for (_uint i = 0; i < slotCount; ++i)
		{
			if (m_vTextureList[i])
				srvs[i] = m_vTextureList[i]->Get_SRV();
		}

		context->PSSetShaderResources(0, slotCount, srvs.data()); 
	}

	static ID3D11SamplerState* gSamplerState = nullptr;

	if (!gSamplerState)
	{
		D3D11_SAMPLER_DESC sampleDesc = {};
		sampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampleDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampleDesc.MinLOD = 0;
		sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;

		if (FAILED(CGraphicDevice::GetInstance().Get_Device()->CreateSamplerState(&sampleDesc, &gSamplerState)))
		{
			CDebug::LogError(L"Create failed SamplerState in Material");
			return;
		}
	}

	context->PSSetSamplers(0, 1, &gSamplerState);
}

void CMaterial::Bind_CameraWithShader(CShader* _shader, const _float3 _camPos, const _fmatrix _view, const _cmatrix _projection, const _uint _boneCount)
{
	ID3D11DeviceContext* context = CGraphicDevice::GetInstance().Get_Context();

	if (_shader)
		_shader->Bind();
	else if (m_pShader)
		m_pShader->Bind();

	Bind_Texture();

	// b1: PerCamera
	CameraCB camCB = {};
	camCB.camPos = _camPos;
	camCB.view = XMMatrixTranspose(_view);
	camCB.proj = XMMatrixTranspose(_projection);
	_matrix viewInv = XMMatrixInverse(nullptr, _view);
	camCB.viewInv = XMMatrixTranspose(viewInv);
	context->UpdateSubresource(m_pCameraBuffer, 0, nullptr, &camCB, 0, 0);
	context->PSSetConstantBuffers(1, 1, &m_pCameraBuffer);
	context->VSSetConstantBuffers(1, 1, &m_pCameraBuffer);

	// b2: PerMaterial
	MaterialCB mat = {};

	mat.baseColor = m_vBaseColor;
	mat.useTexture = (!m_vTextureList.empty() && m_vTextureList[0] != nullptr);
	mat.boneCount = _boneCount;

	context->UpdateSubresource(m_pMaterialBuffer, 0, nullptr, &mat, 0, 0);
	context->PSSetConstantBuffers(2, 1, &m_pMaterialBuffer);
	context->VSSetConstantBuffers(2, 1, &m_pMaterialBuffer);

	if (m_vCustomBufferByteList.size() > 0)
		Bind_CustomValues();
}
