#include "epch.h"
#include "Renderer.h"

CRenderer::CRenderer()
	: m_pMaterial(nullptr)
	, m_pOutlineMat(nullptr)
	, m_bUseInstancing(false)
	, m_vInstanceWorlds({})
{
}

CRenderer::~CRenderer()
{
}

void CRenderer::OnDestroy()
{
	Safe_Release(m_pMaterial);
	Safe_Release(m_pOutlineMat);
}

HRESULT CRenderer::Initialize()
{
	if (FAILED(__super::Initialize()))
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

const _bool CRenderer::Is_UseInstancing() const
{
	return m_bUseInstancing;
}

void CRenderer::Set_UseInstancing(const _bool _value)
{
	m_bUseInstancing = _value;
}

void CRenderer::SetInstanceWorlds(const vector<_float4x4>& _worlds)
{
	m_vInstanceWorlds = _worlds;
}

void CRenderer::AddInstanceWorld(const _float4x4& _w)
{
	m_vInstanceWorlds.push_back(_w);
}

const size_t CRenderer::GetInstanceCount() const
{
	return m_vInstanceWorlds.size();
}

const _bool CRenderer::HasInstances() const
{
	return !m_vInstanceWorlds.empty();
}
