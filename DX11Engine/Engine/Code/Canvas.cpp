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
		m_pRectMesh = CResources::GetInstance().LoadOnGame<CMeshBuffer>(L"LineRect (MeshBuffer)");
		m_pRectMesh->AddRef();
	}

	if (!m_pLineMat)
	{
		m_pLineMat = CResources::GetInstance().LoadOnGame<CMaterial>(L"DefaultLineMaterial (Material)");
		m_pLineMat->AddRef();
	}

	return S_OK;
}

void CCanvas::OnPreRender_Editor()
{
	if (!m_pRectMesh)
		return;
	
	Get_Transform()->Set_LocalScale(10.f, 5.f, 0.f);
}

void CCanvas::Render_Editor()
{
	CCamera* cam = CSceneManager::GetInstance().Get_EditorCamera();

	_matrix matWorld = Get_Transform()->Get_WorldMatrix();
	_matrix matView = cam->Get_ViewMatrix();
	_matrix matProj = cam->Get_ProjectionMatrix();

	if (m_pLineMat)
		m_pLineMat->Bind(matWorld, matView, matProj, 0);

	if (m_pRectMesh)
		m_pRectMesh->Render();
}

void CCanvas::OnPostRender_Editor()
{
}

void CCanvas::OnDestroy()
{
	Safe_Release(m_pRectMesh);
	Safe_Release(m_pLineMat);
}
