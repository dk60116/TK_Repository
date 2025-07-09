#include "epch.h"
#include "Canvas.h"

CCanvas::CCanvas()
	: m_eRenderMode(RenderMode::ScreenSpace_Overlay)
	, m_lUIObjectList({})
	, m_pRectMesh(nullptr)
	, m_pLineMat(nullptr)
{
}

CCanvas::~CCanvas()
{
}

CCanvas* CCanvas::Create()
{
	return new CCanvas();
}

HRESULT CCanvas::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (!m_pRectMesh)
	{
		m_pRectMesh = CResources::GetInstance().LoadOnGame<CMeshBuffer>(L"Quad (MeshBuffer)");
		m_pRectMesh->AddRef();
	}

	if (m_pRectMesh && !m_pLineMat)
	{
	}

	if (m_pLineMat)
		m_pLineMat->Set_Shader(CResources::GetInstance().LoadOnScene<CShader>(L"DefaultLine (Shader)"));

	return S_OK;
}

void CCanvas::Render_Editor()
{
}

void CCanvas::OnDestroy()
{

	Safe_Release(m_pRectMesh);
	Safe_Release(m_pLineMat);
}
