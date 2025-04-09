#include "pch.h"
#include "CMainApp.h"
#include "CTimeMgr.h"
#include "CFrameMgr.h"
#include "CManagement.h"
#include "CMainScene.h"

CMainApp::CMainApp()
	: m_pDevClass(nullptr)
	, m_pGraphicDev(nullptr)
	, m_pVB(nullptr)
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp()
{
	if (FAILED(Engine::CGraphicDev::GetInstance()->Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDevClass)))
		return E_FAIL;

	CTimeMgr::GetInstance().Ready_Timer();

	m_pGraphicDev = m_pDevClass->Get_GraphicDev();
	m_pGraphicDev->AddRef();

	CManagement::GetInstance().SetGraphicDevice(m_pGraphicDev);

	CMainScene* mainScene = new CMainScene();
	CManagement::GetInstance().CreateScene(mainScene, L"MainScene");
	CManagement::GetInstance().ChangeScene(L"MainScene");

	if (FAILED(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXCOL) * 3, 0, FVF_COL, D3DPOOL_DEFAULT, &m_pVB, NULL)))
		return E_FAIL;

	return S_OK;
}

_int CMainApp::Update_MainApp()
{
	CManagement::GetInstance().getCrtScene()->Update();

	return 0;
}

void CMainApp::LateUpdate_MainApp()
{
}

void CMainApp::Render_MainApp()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	m_pDevClass->Render_Begin(D3DCOLOR_ARGB(49, 77, 121, 255));

	CManagement::GetInstance().getCrtScene()->Render();

	m_pDevClass->Render_End();
}

CMainApp* CMainApp::Create()
{
	CMainApp* pMainApp = new CMainApp;

	if (FAILED(pMainApp->Ready_MainApp()))
	{
		Engine::Safe_Release(pMainApp);
		return nullptr;
	}

	return pMainApp;
}

void CMainApp::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pGraphicDev);

	CFrameMgr::GetInstance()->DestroyInstance();
	m_pDevClass->DestroyInstance();
}
