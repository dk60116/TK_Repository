#include "pch.h"
#include "CMainProcess.h"
#include "CFrameMgr.h"
#include "CResources.h"
#include "CMainScene.h"

CMainProcess::CMainProcess()
	: m_pDevClass(nullptr)
	, m_pGraphicDev(nullptr)
	, m_pVB(nullptr)
	, m_eGameState(RUNNING)
	, m_bStepOneFrame(false)
{
}

CMainProcess::~CMainProcess()
{
	Release();
}

HRESULT CMainProcess::Ready_MainApp()
{
	if (FAILED(CGraphicDev::GetInstance().Ready_GraphicDev
	(
		CScreen::GetInstance().getGameHandle(), MODE_WIN,
		CScreen::GetInstance().getResolution().x, CScreen::GetInstance().getResolution().y, 
		&m_pDevClass
	)))
		return E_FAIL;

#ifdef  _DEBUG
	CDebug::Init();
#endif

	m_pGraphicDev = m_pDevClass->Get_GraphicDev();
	m_pGraphicDev->AddRef();

	CTimeMgr::GetInstance().Ready_Timer();

	CResources::GetInstance().LoadAllFiles(m_pGraphicDev);

	CManagement::GetInstance().SetGraphicDevice(m_pGraphicDev);

	CMainScene* mainScene = new CMainScene();
	CManagement::GetInstance().CreateScene(mainScene, L"MainScene");
	CManagement::GetInstance().ChangeScene(L"MainScene");

	return S_OK;
}

_int CMainProcess::Update_MainApp()
{
	CTimeMgr::GetInstance().Update();

	wstring fpsTxt = L"FPS: " + to_wstring(CTimeMgr::GetInstance().Get_FPS());
	SetWindowText(CScreen::GetInstance().getMainHandle(), fpsTxt.c_str());

	CInput::GetInstance().Update();
	CManagement::GetInstance().getCrtScene()->Update();

	Render_MainApp();

	CManagement::GetInstance().getCrtScene()->LateUpdate();
	CInput::GetInstance().LateUpdate();

	m_bStepOneFrame = false;

	return 0;
}

void CMainProcess::Render_MainApp()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, CManagement::GetInstance().getCrtScene()->getOptions().lighting);

	m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	m_pDevClass->Render_Begin(D3DCOLOR_XRGB(49, 77, 121));

	CManagement::GetInstance().getCrtScene()->Render();

	m_pDevClass->Render_End();
}

void CMainProcess::Release()
{
}

void CMainProcess::OnScreenChange(const _uint& _width, const _uint& _height)
{
	if (!m_pGraphicDev)
		return;

	if (FAILED(m_pDevClass->ReSize(_width, _height)))
		return;

	CScreen::GetInstance().UpdateResolution(_width, _height);

	CManagement::GetInstance().getCrtScene()->UpdateAllCameraResolution();
}
