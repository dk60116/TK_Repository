#include "pch.h"
#include "CMainProcess.h"
#include "CResources.h"
#include "CMainScene.h"

CMainProcess::CMainProcess()
	: m_pDevClass(nullptr)
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
		CScreen::GetInstance().getSceneHandle(), MODE_WIN,
		CScreen::GetInstance().getGameResolution().x, CScreen::GetInstance().getGameResolution().y,
		&m_pDevClass
	)))
		return E_FAIL;

#ifdef  _DEBUG
	CDebug::Init();
#endif

	CTimeMgr::GetInstance().Ready_Timer();

	CResources::GetInstance().LoadAllFiles(m_pDevClass->Get_GraphicDev());

	CManagement::GetInstance().SetGraphicDevice(m_pDevClass->Get_GraphicDev());

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

	if (GetForegroundWindow() == CScreen::GetInstance().getSceneHandle())
		CManagement::GetInstance().getCrtScene()->UpdateEditor();
	CManagement::GetInstance().getCrtScene()->Update();

	Render_MainApp();

	if (GetForegroundWindow() == CScreen::GetInstance().getSceneHandle())
		CManagement::GetInstance().getCrtScene()->LateUpdateEditor();
	CManagement::GetInstance().getCrtScene()->LateUpdate();
	CInput::GetInstance().LateUpdate();

	m_bStepOneFrame = false;

	return 0;
}

void CMainProcess::Render_MainApp()
{
	m_pDevClass->Get_GraphicDev()->SetRenderState(D3DRS_LIGHTING, CManagement::GetInstance().getCrtScene()->getOptions().lighting);

	m_pDevClass->Get_GraphicDev()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	D3DVIEWPORT9 sceneViewport = {};
	sceneViewport.X = 0;
	sceneViewport.Y = 0;
	sceneViewport.Width = CScreen::GetInstance().getSceneResolution().x;
	sceneViewport.Height = CScreen::GetInstance().getSceneResolution().y;
	sceneViewport.MinZ = 0.0f;
	sceneViewport.MaxZ = 1.0f;

	D3DVIEWPORT9 gameViewport = {};
	gameViewport.X = 0;
	gameViewport.Y = 0;
	gameViewport.Width = CScreen::GetInstance().getGameResolution().x;
	gameViewport.Height = CScreen::GetInstance().getGameResolution().y;
	gameViewport.MinZ = 0.0f;
	gameViewport.MaxZ = 1.0f;

	m_pDevClass->Render_Begin(sceneViewport, D3DCOLOR_XRGB(50, 50, 50));

	CManagement::GetInstance().getCrtScene()->Render_CScene();

	m_pDevClass->Render_End(CScreen::GetInstance().getSceneHandle());

	m_pDevClass->Render_Begin(gameViewport, D3DCOLOR_XRGB(49, 77, 121));

	CManagement::GetInstance().getCrtScene()->Render_Game();

	m_pDevClass->Render_End(CScreen::GetInstance().getGameHandle());
}

void CMainProcess::Release()
{
}

void CMainProcess::OnSceneScreenChange(const _uint& _width, const _uint& _height)
{
	if (!m_pDevClass || !m_pDevClass->Get_GraphicDev())
		return;

	//CScreen::GetInstance().UpdateSceneResolution(_width, _height);

	CManagement::GetInstance().getCrtScene()->UpdateSceneCameraResolution(vector2Int(_width, _height));
}

void CMainProcess::OnGameScreenChange(const _uint& _width, const _uint& _height)
{
	if (!m_pDevClass || !m_pDevClass->Get_GraphicDev())
		return;

	//CScreen::GetInstance().UpdateGameResolution(_width, _height); 

	CManagement::GetInstance().getCrtScene()->UpdateAllCameraResolution(vector2Int(_width, _height));
}
