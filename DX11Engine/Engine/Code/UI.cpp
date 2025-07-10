#include "epch.h"
#include "UI.h"

CUI::CUI()
	: m_pRectTransform(nullptr)
	, m_pCanvas(nullptr)
	, m_pRectGizmoMesh(nullptr)
	, m_pLineMat(nullptr)
	, m_pRectMesh(nullptr)
	, m_pMaterial(nullptr)
{
}

CUI::~CUI()
{
	OnDestroy();
}

HRESULT CUI::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

#ifndef _CLIENT_BUILD
	if (!m_pRectGizmoMesh)
	{
		m_pRectGizmoMesh = CResources::GetInstance().LoadOnGame<CMeshBuffer>(L"LineRect (Mesh Buffer)");
		m_pRectGizmoMesh->AddRef();
	}

	if (!m_pLineMat)
	{
		m_pLineMat = CResources::GetInstance().LoadOnGame<CMaterial>(L"DefaultLineMaterial (Material)");
		m_pLineMat->AddRef();
	}
#endif

	if (!m_pRectTransform)
	{
		m_pRectTransform = m_pGameObject->AddComponent<CRectTransform>();
		
		if (m_pRectTransform)
		{
			m_pGameObject->Set_Transform(m_pRectTransform);
			m_pRectTransform->AddRef();
		}
	}

	if (!m_pRectMesh)
		Set_Mesh(CResources::GetInstance().LoadOnGame<CMeshBuffer>(L"Quad (Mesh Buffer)"));

	if (!m_pMaterial)
		Set_Material(CResources::GetInstance().LoadOnGame<CMaterial>(L"DefaultUIMaterial (Material)"));

	return S_OK;
}

void CUI::OnDestroy()
{
	Safe_Release(m_pRectGizmoMesh);
	Safe_Release(m_pLineMat);

	Safe_Release(m_pRectTransform);
	Safe_Release(m_pRectMesh);
	Safe_Release(m_pMaterial);
	Safe_Release(m_pCanvas);
}

void CUI::Set_Mesh(CMeshBuffer* _mesh)
{
	if (_mesh == m_pRectMesh)
		return;

	Safe_Release(m_pMaterial);

	m_pRectMesh = _mesh;

	if (m_pRectMesh)
		m_pRectMesh->AddRef();
}

void CUI::Set_Material(CMaterial* _material)
{
	if (_material == m_pMaterial)
		return;

	Safe_Release(m_pMaterial);

	m_pMaterial = _material;

	if (m_pMaterial)
		m_pMaterial->AddRef();
}
