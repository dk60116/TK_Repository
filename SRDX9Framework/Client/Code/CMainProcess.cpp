#include "pch.h"
#include "CMainProcess.h"
#include "CResources.h"
#include "CMainScene.h"

CMainProcess::CMainProcess()
	: m_pDevClass(nullptr)
	 , m_iDPI(1)
{
}

CMainProcess::~CMainProcess()
{
	Release();
}

HRESULT CMainProcess::Ready_MainApp()
{
	OnSceneScreenChange(CScreen::GetInstance().getSceneResolution().x, CScreen::GetInstance().getSceneResolution().y);
	OnGameScreenChange(CScreen::GetInstance().getGameResolution().x, CScreen::GetInstance().getGameResolution().y);

	CalcDPI();

	if (FAILED(CGraphicDev::GetInstance().Ready_GraphicDev
	(
		CEngineEditor::GetInstance().FindWindowHandle(L"Game"), MODE_WIN,
		m_iDPI, m_iDPI - CHILDTOPBARHEIGHT,
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

	return S_OK;
}

_int CMainProcess::Update_MainApp()
{
	CTime::GetInstance().Update();

	wstring fpsTxt = L"FPS: " + to_wstring(CTime::GetInstance().Get_FPS());
	SetWindowText(CEngineEditor::GetInstance().FindWindowHandle(L"Base"), fpsTxt.c_str());

	CInput::GetInstance().Update();
	
	CEngineEditor::GetInstance().Update();
	CManagement::GetInstance().EditorUpdate();

	if (CEngineEditor::GetInstance().isPlaying())
	{
		if (!CEngineEditor::GetInstance().isPaused() || CEngineEditor::GetInstance().isNextFrame())
		{
			CManagement::GetInstance().getCrtScene()->Update();
			CManagement::GetInstance().getCrtScene()->LateUpdate();
		}
	}

	Render_MainApp();
	
	CManagement::GetInstance().getCrtScene()->LateUpdateEditor();
	
	CInput::GetInstance().LateUpdate();

	CEngineEditor::GetInstance().SetNextFrame(false);

	return 0;
}

void CMainProcess::Render_MainApp()
{
	m_pDevClass->Get_GraphicDev()->SetRenderState(D3DRS_LIGHTING, CManagement::GetInstance().getCrtScene()->getOptions().lighting);

	D3DVIEWPORT9 sceneVP = {};
	sceneVP.X = 0;
	sceneVP.Y = CHILDTOPBARHEIGHT;
	sceneVP.Width = m_iDPI;
	sceneVP.Height = m_iDPI - CHILDTOPBARHEIGHT;
	sceneVP.MinZ = 0.0f;
	sceneVP.MaxZ = 1.0f;

	m_pDevClass->Render_Begin(sceneVP, D3DCOLOR_XRGB(50, 50, 50));

	CManagement::GetInstance().getCrtScene()->Render_Editor();

	m_pDevClass->Render_End(CEngineEditor::GetInstance().FindWindowHandle(L"Scene"));

	D3DVIEWPORT9 gameVP = {};
	gameVP.X = 0;
	gameVP.Y = CHILDTOPBARHEIGHT;
	gameVP.Width = m_iDPI;
	gameVP.Height = m_iDPI - CHILDTOPBARHEIGHT;
	gameVP.MinZ = 0.0f;
	gameVP.MaxZ = 1.0f;

	m_pDevClass->Render_Begin(gameVP, D3DCOLOR_XRGB(49, 77, 121));

	CManagement::GetInstance().getCrtScene()->Render_Game();

	m_pDevClass->Render_End(CEngineEditor::GetInstance().FindWindowHandle(L"Game"));
}

void CMainProcess::Release()
{
}

void CMainProcess::OnSceneScreenChange(const _uint _width, const _uint _height)
{
	if (!m_pDevClass || !m_pDevClass->Get_GraphicDev())
		return;

	//CScreen::GetInstance().UpdateSceneResolution(_width, _height);

	CManagement::GetInstance().getCrtScene()->UpdateSceneCameraResolution(vector2Int(_width, _height - CHILDTOPBARHEIGHT));
}

void CMainProcess::OnGameScreenChange(const _uint _width, const _uint _height)
{
	if (!m_pDevClass || !m_pDevClass->Get_GraphicDev())
		return;

	CGraphicDev::GetInstance().ReSize(m_iDPI, m_iDPI - CHILDTOPBARHEIGHT);

	//CScreen::GetInstance().UpdateGameResolution(_width, _height);
	
	CalcDPI();

	CManagement::GetInstance().getCrtScene()->UpdateAllCameraResolution(vector2Int(_width, _height));
}

void CMainProcess::CalcDPI()
{
	vector2Int resolution = CScreen::GetInstance().getGameResolution();

	if (resolution.x >= resolution.y)
		m_iDPI = resolution.x;
	else
		m_iDPI = resolution.y;
}
