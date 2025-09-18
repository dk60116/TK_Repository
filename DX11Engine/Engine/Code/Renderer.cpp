#include "epch.h"
#include "Renderer.h"

CRenderer::CRenderer()
	: m_pMaterial(nullptr)
	, m_pOutlineMat(nullptr)
	, m_fScaleFactor(1.f)
	, m_bNoneCulling(false)
{
}

CRenderer::~CRenderer()
{
}

void CRenderer::OnPostRender_Editor()
{
	ResetShaderResources();
}

void CRenderer::OnPostRender()
{
	ResetShaderResources();
}

void CRenderer::OnDestroy()
{
	Safe_Release(m_pMaterial);
	Safe_Release(m_pOutlineMat);
}

HRESULT CRenderer::Initialize(void* _desc)
{
	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	if (!m_pMaterial)
	{
		Set_Material(CResources::CloneOnGame<CMaterial>(L"LitMaterial (Material)"));
	}

	if (!m_pOutlineMat)
	{

	}
	
	if (m_pOutlineMat)
		m_pOutlineMat->AddRef();

	CShader* outShader = CResources::LoadOnGame<CShader>(L"Outline (Shader)");

	if (!outShader)
	{
		CDebug::LogError("Not found outline shader");
		return E_FAIL;
	}

	//m_pOutlineMat->Set_Shader(outShader);

	return S_OK;
}

CMaterial* CRenderer::Get_Material()
{
	return m_pMaterial;
}

void CRenderer::Set_Material(CMaterial* _material)
{
	Safe_Release(m_pMaterial);

	m_pMaterial = _material;

	if (m_pMaterial)
		m_pMaterial->AddRef();
}

void CRenderer::SetNoneCull(const _bool _value)
{
	m_bNoneCulling = _value;
}

void CRenderer::Bind_InstanceData(_fmatrix matWorld, CMeshBuffer* pBuffer)
{
	auto& inst = pBuffer->Get_InstancingDesc();

	if (inst.dcapacity < 1)
	{
		if (FAILED(pBuffer->CreateInstanceBuffer(1, D3D11_USAGE_DYNAMIC)))
		{
			CDebug::LogError(L"MeshRenderer: failed to create instance buffer for " + m_pGameObject->Get_ObjectNameID());
			return;
		}
	}

	if (inst.data.size() < 1)
		inst.data.resize(1);

	_float4x4 w;
	XMStoreFloat4x4(&w, matWorld);
	inst.data[0] = MakeInstanceData(w);
	inst.count = 1;

	pBuffer->UpdateInstanceBuffer();
}

void CRenderer::ResetShaderResources()
{
}
