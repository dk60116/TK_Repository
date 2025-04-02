#include "CMianScene.h"
#include "CImage.h"
#include "CSceneManager.h"
#include "CCanvas.h"
#include "CText.h"
#include "CButton.h"
#include "CSceneManager.h"

CMianScene::CMianScene()
{
	m_strName = L"Main Scene";
}

CMianScene::~CMianScene()
{
	Release();
}

void CMianScene::Awake()
{
	Awake_();

	// 카메라
	CGameObject* camObj = AddObject(L"Main Camera", Layer::Default);
	CCamera* mainCam = dynamic_cast<CCamera*>(camObj->AddComponent(new CCamera()));

	// 캔버스
	CGameObject* canvasObj = AddObject(L"Canvas", Layer::UI);
	canvasObj->AddComponent(new CCanvas());
	CCanvas* canvas = canvasObj->GetComponent<CCanvas>();

	// 메인메뉴
	CGameObject* mainMenuUIObj = new CGameObject();
	mainMenuUIObj->SetName(L"MainMenu UI");
	mainMenuUIObj->SetLayer(Layer::UI);
	CImage* mainUIImg = dynamic_cast<CImage*>(mainMenuUIObj->AddComponent(new CImage(L"UI_MainUI")));
	mainUIImg->SetSpriteArea(vector2Int::zero(), vector2Int(528, 327));
	mainUIImg->getRect().SetSize(vector2(250, 200));
	canvas->InsertUI(mainUIImg);

	CGameObject* mainMenuTextObj = new CGameObject();
	mainMenuTextObj->SetName(L"MainMenu UI Text");
	mainMenuTextObj->SetLayer(Layer::UI);
	CText* mainUITxt = dynamic_cast<CText*>(mainMenuTextObj->AddComponent(new CText()));
	mainUITxt->SetText(L"메인메뉴");
	mainUITxt->getRect().SetSize(vector2(100.f, 50.f));
	mainUITxt->getRect().SetAnchorPreset(vector2(-1, 1));
	mainUITxt->getRect().SetChildPos(vector2(40.f, 17.f));
	mainUITxt->getOption().fontSize = 22;
	mainUITxt->getOption().bold = true;
	mainUITxt->getOption().color = ColorValue(246, 222, 186);
	canvas->InsertUI(mainUITxt);
	mainUITxt->SetParent(mainUIImg);

	// 시작버튼
	CGameObject* mainMenuStartBtnObj = new CGameObject();
	mainMenuStartBtnObj->SetName(L"MainMenu UI StartBtn");
	mainMenuStartBtnObj->SetLayer(Layer::UI);
	mainMenuStartBtnObj->AddComponent(new CButton(L"UI_MainBtn"));
	CButton* mainUIStartBtn = mainMenuStartBtnObj->GetComponent<CButton>();
	mainUIStartBtn->getRect().SetSize(vector2(230.f, 50.f));
	mainUIStartBtn->SetSpriteArea(vector2Int(160, 0), vector2Int(160, 25));
	mainUIStartBtn->SetParent(mainUIImg);
	mainUIStartBtn->getRect().SetChildPos(vector2::up() * 37.5f);
	canvas->InsertUI(mainUIStartBtn);
	mainUIStartBtn->getRect().SetChildPos(vector2::up() * 37.5f);
	mainUIStartBtn->AddOnClickEvent([this]()
		{
			GoGameScene();
		});

	CGameObject* mainMenuStartBtnTextObj = new CGameObject();
	mainMenuStartBtnTextObj->SetName(L"MainMenu Enter Text");
	mainMenuStartBtnTextObj->SetLayer(Layer::UI);
	CText* mainMenuStartBtnText = dynamic_cast<CText*>(mainMenuStartBtnTextObj->AddComponent(new CText()));
	mainMenuStartBtnText->SetText(L"게임 시작");
	mainMenuStartBtnText->getRect().SetSize(vector2(100.f, 50.f));
	mainMenuStartBtnText->getOption().fontSize = 24;
	mainMenuStartBtnText->getOption().bold = true;
	canvas->InsertUI(mainMenuStartBtnText);
	mainMenuStartBtnText->SetParent(mainUIStartBtn);

	// 종료버튼
	CGameObject* mainMenuExitBtnObj = new CGameObject();
	mainMenuExitBtnObj->SetName(L"MainMenu UI ExitBtn");
	mainMenuExitBtnObj->SetLayer(Layer::UI);
	mainMenuExitBtnObj->AddComponent(new CButton(L"UI_MainBtn_Red"));
	CButton* mainUIExitBtn = mainMenuExitBtnObj->GetComponent<CButton>();
	mainUIExitBtn->getRect().SetSize(vector2(230.f, 50.f));
	mainUIExitBtn->SetSpriteArea(vector2Int(160, 0), vector2Int(160, 25));
	mainUIExitBtn->SetParent(mainUIImg);
	mainUIExitBtn->getRect().SetChildPos(vector2::down() * 27.5f);
	canvas->InsertUI(mainUIExitBtn);

	// 종료 텍스트
	CGameObject* mainMenuExitBtnTextObj = new CGameObject();
	mainMenuExitBtnTextObj->SetName(L"MainMenu Exit Text");
	mainMenuExitBtnTextObj->SetLayer(Layer::UI);
	CText* mainMenuExitBtnText = dynamic_cast<CText*>(mainMenuExitBtnTextObj->AddComponent(new CText()));
	mainMenuExitBtnText->SetText(L"게임 종료");
	mainMenuExitBtnText->getRect().SetSize(vector2(100.f, 50.f));
	mainMenuExitBtnText->getOption().fontSize = 24;
	mainMenuExitBtnText->getOption().bold = true;
	canvas->InsertUI(mainMenuExitBtnText);
	mainMenuExitBtnText->SetParent(mainUIExitBtn);
}

void CMianScene::Start()
{
	Start_();
}

void CMianScene::Update()
{
	Update_();
}

void CMianScene::FixedUpdate()
{
	FixedUpdate_();
}

void CMianScene::LateUpdate()
{
	LateUpdate_();
}

void CMianScene::Render(HDC _hDC)
{
	Render_(_hDC);
}

void CMianScene::Release()
{
}

void CMianScene::GoGameScene()
{
	CSceneManager::GetInstance().LoadScene(L"Test Scene");
}
