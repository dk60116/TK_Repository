#include "epch.h"
#include "Canvas.h"

CCanvas::CCanvas()
	: m_eRenderMode(RenderMode::ScreenSpace_Overlay)
	, m_lUIObjectList({})
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

	return S_OK;
}

void CCanvas::OnPreRender_Editor()
{
	if (!m_pRectGizmoMesh)
		return;

	vector2 resolution = vector2(CDisplay::GetInstance().Get_ScreenResolution().x, CDisplay::GetInstance().Get_ScreenResolution().y);
	
	if (m_eRenderMode == RenderMode::ScreenSpace_Overlay)
	{
		Get_Transform()->Set_Position(10.f, 10.f, 0.f);
		Get_Transform()->Set_EulerAngles(vector3::zero());
		Get_Transform()->Set_LocalScale(resolution.x * 0.01f, resolution.y * 0.01f, 1.f);
	}
}

void CCanvas::Render_Editor()
{
	CCamera* cam = CSceneManager::GetInstance().Get_EditorCamera();

	_matrix matWorld = Get_Transform()->Get_WorldMatrix();
	_matrix matView = cam->Get_ViewMatrix();
	_matrix matProj = cam->Get_ProjectionMatrix();

	if (m_pLineMat)
		m_pLineMat->Bind(matWorld, matView, matProj, 0);

	if (m_pRectGizmoMesh)
		m_pRectGizmoMesh->Render();
}

void CCanvas::OnPostRender_Editor()
{

}

void CCanvas::OnDestroy()
{
	for (TRAVERSAL_ITER(m_lUIObjectList, it))
		Safe_Release(*it);

	m_lUIObjectList.clear();
}
