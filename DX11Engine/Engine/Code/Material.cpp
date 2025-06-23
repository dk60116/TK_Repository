#include "epch.h"
#include "Material.h"

CMaterial::CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(nullptr)
	, m_pContext(nullptr)
	, m_pPixelShader(nullptr)
	, m_pInputLayout(nullptr)
	, m_pConstantBuffer(nullptr)
	, m_pDiffuseSRV(nullptr)
	, m_vDiffuseColor(ColorValue::white())
{
}

CMaterial::~CMaterial()
{
	OnDestroy();
}

HRESULT CMaterial::Initialize(const std::wstring& vsPath, const std::wstring& psPath)
{
	m_pDevice = CGraphicDevice::GetInstance().Get_Device();
	m_pContext = CGraphicDevice::GetInstance().Get_Context();

	m_pDevice->AddRef();
	m_pContext->AddRef();

	return S_OK;
}

void CMaterial::OnDestroy()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

void CMaterial::Render()
{
	Bind_Shader();
	Bind_Texture();

	struct MaterialCB
	{
		XMFLOAT4 diffuseColor;
		BOOL useTexture;
		XMFLOAT3 padding;
	};

	MaterialCB cbData = {};

	cbData.diffuseColor = 
	{
		m_vDiffuseColor.r / 255.f,
		m_vDiffuseColor.g / 255.f,
		m_vDiffuseColor.b / 255.f,
		m_vDiffuseColor.a / 255.f
	};

	cbData.useTexture = m_pDiffuseSRV != nullptr;

	m_pContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cbData, 0, 0);
	m_pContext->PSSetConstantBuffers(1, 1, &m_pConstantBuffer);
}

void CMaterial::Set_DiffuseColor(const ColorValue& color)
{
}

void CMaterial::Set_DiffuseTexture(ID3D11ShaderResourceView* pSRV)
{
}

HRESULT CMaterial::Load_Shader(const std::wstring& vsPath, const std::wstring& psPath)
{
	return E_NOTIMPL;
}

HRESULT CMaterial::Create_ConstantBuffer()
{
	return E_NOTIMPL;
}

void CMaterial::Bind_Shader()
{
}

void CMaterial::Bind_Texture()
{
}
