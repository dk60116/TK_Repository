#include "pch.h"
#include "CMainProcess.h"
#include "CResources.h"
#include "CMainScene.h"

CMainProcess::CMainProcess()
	: m_pDevClass(nullptr)
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
		CScreen::GetInstance().getWindowHandle(L"Game"), MODE_WIN,
		CScreen::GetInstance().getGameResolution().x, CScreen::GetInstance().getGameResolution().y,
		&m_pDevClass
	)))
		return E_FAIL;

#ifdef  _DEBUG
	CDebug::Init();
#endif

	CTime::GetInstance().Ready_Timer();

	CResources::GetInstance().LoadAllFiles(m_pDevClass->Get_GraphicDev());

	CManagement::GetInstance().SetGraphicDevice(m_pDevClass->Get_GraphicDev());

	CMainScene* mainScene = new CMainScene();
	CManagement::GetInstance().CreateScene(mainScene, L"MainScene");
	CManagement::GetInstance().LoadScene(L"MainScene");

	OnSceneScreenChange(CScreen::GetInstance().getSceneResolution().x, CScreen::GetInstance().getSceneResolution().y );
	OnGameScreenChange(CScreen::GetInstance().getGameResolution().x, CScreen::GetInstance().getGameResolution().y);

	return S_OK;
}

_int CMainProcess::Update_MainApp()
{
	CTime::GetInstance().Update();

	wstring fpsTxt = L"FPS: " + to_wstring(CTime::GetInstance().Get_FPS());
	SetWindowText(CScreen::GetInstance().getWindowHandle(L"Base"), fpsTxt.c_str());

	CInput::GetInstance().Update();

	if (GetForegroundWindow() == CScreen::GetInstance().getWindowHandle(L"Scene"))
		CManagement::GetInstance().EditorUpdate();

	if (CEngineEditor::GetInstance().isPlaying())
	{
		if (!CEngineEditor::GetInstance().isPaused() || CEngineEditor::GetInstance().isNextFrame())
			CManagement::GetInstance().getCrtScene()->Update();
	}

	Render_MainApp();

	if (GetForegroundWindow() == CScreen::GetInstance().getWindowHandle(L"Scene"))
		CManagement::GetInstance().getCrtScene()->LateUpdateEditor();

	CManagement::GetInstance().getCrtScene()->LateUpdate();
	
	CInput::GetInstance().LateUpdate();

	CEngineEditor::GetInstance().SetNextFrame(false);

	return 0;
}

void CMainProcess::Render_MainApp()
{
	m_pDevClass->Get_GraphicDev()->SetRenderState(D3DRS_LIGHTING, CManagement::GetInstance().getCrtScene()->getOptions().lighting);

	CGraphicDev::GetInstance().ReSize(CScreen::GetInstance().getSceneResolution().x, CScreen::GetInstance().getSceneResolution().y);

	D3DVIEWPORT9 sceneViewport = {};
	sceneViewport.X = 0;
	sceneViewport.Y = CHILDTOPBARHEIGHT;
	sceneViewport.Width = CScreen::GetInstance().getSceneResolution().x;
	sceneViewport.Height = CScreen::GetInstance().getSceneResolution().y - CHILDTOPBARHEIGHT;
	sceneViewport.MinZ = 0.0f;
	sceneViewport.MaxZ = 1.0f;

	m_pDevClass->Render_Begin(sceneViewport, D3DCOLOR_XRGB(50, 50, 50));

	CManagement::GetInstance().getCrtScene()->Render_Editor();

	auto b = CScreen::GetInstance().getWindowHandle(L"Base");

	m_pDevClass->Render_End(CScreen::GetInstance().getWindowHandle(L"Scene"));

	CGraphicDev::GetInstance().ReSize(CScreen::GetInstance().getGameResolution().x, CScreen::GetInstance().getGameResolution().y);

	D3DVIEWPORT9 gameViewport = {};
	gameViewport.X = 0;
	gameViewport.Y = CHILDTOPBARHEIGHT;
	gameViewport.Width = CScreen::GetInstance().getGameResolution().x;
	gameViewport.Height = CScreen::GetInstance().getGameResolution().y - CHILDTOPBARHEIGHT;
	gameViewport.MinZ = 0.0f;
	gameViewport.MaxZ = 1.0f;

	m_pDevClass->Render_Begin(gameViewport, D3DCOLOR_XRGB(49, 77, 121));

	CManagement::GetInstance().getCrtScene()->Render_Game();

	m_pDevClass->Render_End(CScreen::GetInstance().getWindowHandle(L"Game"));
}

void CMainProcess::Release()
{
}

void CMainProcess::OnSceneScreenChange(const _uint _width, const _uint _height)
{
	if (!m_pDevClass || !m_pDevClass->Get_GraphicDev())
		return;

	CScreen::GetInstance().UpdateSceneResolution(_width, _height);

	CManagement::GetInstance().getCrtScene()->UpdateSceneCameraResolution(vector2Int(_width, _height - CHILDTOPBARHEIGHT));
}

void CMainProcess::OnGameScreenChange(const _uint _width, const _uint _height)
{
	if (!m_pDevClass || !m_pDevClass->Get_GraphicDev())
		return;

	CScreen::GetInstance().UpdateGameResolution(_width, _height);

	CManagement::GetInstance().getCrtScene()->UpdateAllCameraResolution(vector2Int(_width, _height - CHILDTOPBARHEIGHT));
}
