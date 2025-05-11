#include "CGameObject.h"
#include "CTestScene.h"
#include "CSceneManager.h"
#include "CSpriteRender_Level.h"
#include "CAnimator.h"
#include "CInput.h"
#include "Jusin_FactorioProject.h"
#include "CTile.h"
#include "CMapManager.h"
#include "CControlManager.h"
#include "CItemManager.h"
#include "CInventory.h"
#include "CCanvas.h"
#include "CImage.h"
#include "CButton.h"
#include "CInventoryUI.h"
#include "CBeltManager.h"
#include "CMonster_Biter.h"

extern bool g_rectRencder;

CTestScene::CTestScene()
	: m_v2ZoomLimit(vector2::zero())
	, m_vTileArchive({})
	, m_pDefaultMap(nullptr)
	, m_pPlayer(nullptr)
	, m_pInventoryUI(nullptr)
	, m_pSoundObj(nullptr)
	, m_vMonsterList({})
	, m_pEndingPanel(nullptr)
	, m_pEndingText(nullptr)
{
	m_strName = L"Test Scene";
}

CTestScene::~CTestScene()
{
	Release();
}

void CTestScene::Awake()
{
	Awake_();

	m_v2ZoomLimit = vector2(0.3f, 1.5f);

	m_pDefaultMap = CMapManager::GetInstance().AttachTileMap(this, nullptr, L"DefaultMap", vector2Int::zero());
	CMapManager::GetInstance().SetMainTile(m_pDefaultMap);
	CTileMap* lake = CMapManager::GetInstance().AttachTileMap(this, m_pDefaultMap, L"Lake", vector2Int(25, 28));
	m_vTileArchive.push_back(lake);
	CTileMap* coal = CMapManager::GetInstance().AttachTileMap(this, m_pDefaultMap, L"CoalGround", vector2Int(40, 30));
	m_vTileArchive.push_back(coal);
	CTileMap* stone = CMapManager::GetInstance().AttachTileMap(this, m_pDefaultMap, L"StoneGround", vector2Int(20, 35));
	m_vTileArchive.push_back(stone);
	CTileMap* iron = CMapManager::GetInstance().AttachTileMap(this, m_pDefaultMap, L"IronGround", vector2Int(30, 20));
	m_vTileArchive.push_back(iron);
	CTileMap* copper = CMapManager::GetInstance().AttachTileMap(this, m_pDefaultMap, L"CopperGround", vector2Int(35, 40));
	m_vTileArchive.push_back(copper);

	// 카메라
	CGameObject* camObj = AddObject(L"PlayerCamera", Layer::Default);
	CCamera* mainCam = dynamic_cast<CCamera*>(camObj->AddComponent(new CCamera()));

	// 캔버스
	CGameObject* canvasObj = AddObject(L"Canvas", Layer::UI);
	canvasObj->AddComponent(new CCanvas());
	CCanvas* canvas = canvasObj->GetComponent<CCanvas>();

	// 사운드
	CGameObject* soundObj = AddObject(L"Sound", Layer::Default);
	m_pSoundObj = dynamic_cast<CAudioSource*>(soundObj->AddComponent(new CAudioSource()));
	m_pSoundObj->SetClip(L"BGM1");
	m_pSoundObj->getOptions().playOnAwake = true;
	m_pSoundObj->getOptions().loop = true;

	// 플레이어
	CGameObject* player = AddObject(L"Player", Layer::Player);
	player->getTransform().SetScale(vector2(1.5f, 2.f));
	m_pPlayer = dynamic_cast<CPlayer*>(player->AddComponent(new CPlayer()));
	CControlManager::GetInstance().SetPlayer(m_pPlayer);
	player->getTransform().SetPosition(vector2::one() * 35.f);
	CAnimator* playerAnim = dynamic_cast<CAnimator*>(player->AddComponent(new CAnimator()));
	player->AddComponent(new CSpriteRenderer(player->GetComponent<CAnimator>()));

	// 대기 모션
	vector2Int idleAnmVecotor = vector2Int(92, 116);
	wstring idleID = L"Player_Idle";
	int idleCount = 22;
	vector2 idleOffset = vector2::zero();
	float idleFI = 0.1f;
	playerAnim->CreateAnimation(L"Player_Idle_Up", idleID, idleCount, idleOffset, idleAnmVecotor, 0, idleFI);
	playerAnim->CreateAnimation(L"Player_Idle_Up_Right", idleID, idleCount, idleOffset, idleAnmVecotor, 1 ,idleFI);
	playerAnim->CreateAnimation(L"Player_Idle_Right", idleID, idleCount, idleOffset, idleAnmVecotor, 2 ,idleFI);
	playerAnim->CreateAnimation(L"Player_Idle_Right_Down", idleID, idleCount, idleOffset, idleAnmVecotor, 3, idleFI);
	playerAnim->CreateAnimation(L"Player_Idle_Down", idleID, idleCount, idleOffset, idleAnmVecotor, 4, idleFI);
	playerAnim->CreateAnimation(L"Player_Idle_Left_Down", idleID, idleCount, idleOffset, idleAnmVecotor, 5, idleFI);
	playerAnim->CreateAnimation(L"Player_Idle_Left", idleID, idleCount, idleOffset, idleAnmVecotor, 6);
	playerAnim->CreateAnimation(L"Player_Idle_Left_Up", idleID, idleCount, idleOffset, idleAnmVecotor, 7, idleFI);

	// 이동 모션
	vector2Int runAnmVecotor = vector2Int(88, 132);
	wstring runID = L"Player_Running";
	int runCount = 22;
	float runFI = 0.02f;
	vector2 runOffset = vector2::zero();
	playerAnim->CreateAnimation(L"Player_Running_Up", runID, runCount, runOffset, runAnmVecotor, 0, runFI);
	playerAnim->CreateAnimation(L"Player_Running_Up_Right", runID, runCount, runOffset, runAnmVecotor, 1, runFI);
	playerAnim->CreateAnimation(L"Player_Running_Right", runID, runCount, runOffset, runAnmVecotor, 2, runFI);
	playerAnim->CreateAnimation(L"Player_Running_Right_Down", runID, runCount, runOffset, runAnmVecotor, 3, runFI);
	playerAnim->CreateAnimation(L"Player_Running_Down", runID, runCount, runOffset, runAnmVecotor, 4, runFI);
	playerAnim->CreateAnimation(L"Player_Running_Left_Down", runID, runCount, runOffset, runAnmVecotor, 5, runFI);
	playerAnim->CreateAnimation(L"Player_Running_Left", runID, runCount, runOffset, runAnmVecotor, 6, runFI);
	playerAnim->CreateAnimation(L"Player_Running_Left_Up", runID, runCount, runOffset, runAnmVecotor, 7, runFI);

	// 채굴 모션
	vector2Int miningAnmVecotor = vector2Int(196, 194);
	wstring MiningID = L"Player_Mining";
	int miningCount = 26;
	float miningFI = 0.02f;
	vector2 miningOffset = vector2::down() * 0.25f;
	playerAnim->CreateAnimation(L"Player_Mining_Up", MiningID, miningCount, miningOffset, miningAnmVecotor, 0, miningFI);
	playerAnim->CreateAnimation(L"Player_Mining_Up_Right", MiningID, miningCount, miningOffset, miningAnmVecotor, 1, miningFI);
	playerAnim->CreateAnimation(L"Player_Mining_Right", MiningID, miningCount, miningOffset, miningAnmVecotor, 2, miningFI);
	playerAnim->CreateAnimation(L"Player_Mining_Right_Down", MiningID, miningCount, miningOffset, miningAnmVecotor, 3, miningFI);
	playerAnim->CreateAnimation(L"Player_Mining_Down", MiningID, miningCount, miningOffset, miningAnmVecotor, 4, miningFI);
	playerAnim->CreateAnimation(L"Player_Mining_Left_Down", MiningID, miningCount, miningOffset, miningAnmVecotor, 5, miningFI);
	playerAnim->CreateAnimation(L"Player_Mining_Left", MiningID, miningCount, miningOffset, miningAnmVecotor, 6, miningFI);
	playerAnim->CreateAnimation(L"Player_Mining_Left_Up", MiningID, miningCount, miningOffset, miningAnmVecotor, 7, miningFI);

	// 전투 모션
	vector2Int gun_IdleAnmVecotor = vector2Int(110, 128);
	wstring gun_IdleID = L"Player_Gun_Idle";
	int gun_IdleCount = 22;
	float gun_IdleFI = 0.1f;
	vector2 gun_IdleOffset = vector2::zero();

	for (int i = 0; i < 8; ++i)
		playerAnim->CreateAnimation(L"Gun_Idle_" + to_wstring(i), gun_IdleID, gun_IdleCount, gun_IdleOffset, gun_IdleAnmVecotor, i, gun_IdleFI);

	vector2Int gun_RunAnmVecotor = vector2Int(108, 136);
	wstring gun_RunID = L"Player_Gun_Run";
	wstring gun_RunFlipID = L"Player_Gun_Run_Flip";
	int gun_RunCount = 22;
	float gun_RunFI = 0.025f;
	vector2 gun_RunOffset = vector2::zero();

	for (int i = 0; i < 36; ++i)
	{
		playerAnim->CreateAnimation(L"Gun_Run_" + to_wstring(i), gun_RunID, gun_RunCount, gun_RunOffset, gun_RunAnmVecotor, i, gun_RunFI);
		playerAnim->CreateAnimation(L"Gun_Run_Flip_" + to_wstring(i), gun_RunFlipID, gun_RunCount, gun_RunOffset, gun_RunAnmVecotor, i, gun_RunFI);
	}

	playerAnim->SetAnimation(L"Player_Idle_Down");
	
	//// 플레이어 마스크
	//CGameObject* playerMask = AddObject(L"Player Shadow", Layer::Player);
	//playerMask->getTransform().SetScale(vector2(1, 2));
	//playerMask->SetParent(player);
	//CAnimator* playerMaskAnim = dynamic_cast<CAnimator*>(playerMask->AddComponent(new CAnimator()));
	//playerMask->AddComponent(new CSpriteRenderer(playerMask->GetComponent<CAnimator>()));
	//playerMask->GetComponent<CSpriteRenderer>()->SetColorTint(ColorValue::Yellow());
	//m_pPlayer->SetMaskAnim(playerMaskAnim);

	//vector2Int idleMaskVector = vector2Int(56, 90);
	//wstring idleMaskID = L"PlayerMask_Idle";
	//vector2 idleMaskOffset = vector2::up() * 0.1f;
	//playerMaskAnim->CreateAnimation(L"Mask_Idle_Up", idleMaskID, idleCount, idleMaskOffset, idleMaskVector, 0, idleFI, true);
	//playerMaskAnim->CreateAnimation(L"Mask_Idle_Up_Right", idleMaskID, idleCount, idleMaskOffset, idleMaskVector, 1, idleFI, true);
	//playerMaskAnim->CreateAnimation(L"Mask_Idle_Right", idleMaskID, idleCount, idleMaskOffset, idleMaskVector, 2, idleFI, true);
	//playerMaskAnim->CreateAnimation(L"Mask_Idle_Right_Down", idleMaskID, idleCount, idleMaskOffset, idleMaskVector, 3, idleFI, true);
	//playerMaskAnim->CreateAnimation(L"Mask_Idle_Down", idleMaskID, idleCount, idleMaskOffset, idleMaskVector, 4, idleFI, true);
	//playerMaskAnim->CreateAnimation(L"Mask_Idle_Left_Down", idleMaskID, idleCount, idleMaskOffset, idleMaskVector, 5, idleFI, true);
	//playerMaskAnim->CreateAnimation(L"Mask_Idle_Left", idleMaskID, idleCount, idleMaskOffset, idleMaskVector, 6, idleFI, true);
	//playerMaskAnim->CreateAnimation(L"Mask_Idle_Left_Up", idleMaskID, idleCount, idleMaskOffset, idleMaskVector, 7, idleFI, true);

	// 플레이어 그림자
	CGameObject* playerShadow = AddObject(L"Player Shadow", Layer::Shadow);
	playerShadow->getTransform().SetScale(vector2(2, 1));
	playerShadow->SetParent(player);
	playerShadow->getTransform().SetChildPosition(vector2(0.75f, 0.5));
	CAnimator* playerShadowAnim = dynamic_cast<CAnimator*>(playerShadow->AddComponent(new CAnimator()));
	playerShadow->AddComponent(new CSpriteRenderer(playerShadow->GetComponent<CAnimator>()));
	playerShadow->GetComponent<CSpriteRenderer>()->SetAlpha(0.4f);
	m_pPlayer->SetShadowAnim(playerShadowAnim);

	vector2Int idleShadowVector = vector2Int(82, 39);
	wstring idleShadowID = L"Shadow_Idle";
	vector2 idleShdowOffset = vector2(-0.12f, 0.2f);
	float idleShdowInterval = 0.1f;
	playerShadowAnim->CreateAnimation(L"Shadow_Idle_Up", idleShadowID, idleCount, idleShdowOffset, idleShadowVector, 0, idleShdowInterval, true);
	playerShadowAnim->CreateAnimation(L"Shadow_Idle_Up_Right", idleShadowID, idleCount, idleShdowOffset, idleShadowVector, 1, idleShdowInterval, true);
	playerShadowAnim->CreateAnimation(L"Shadow_Idle_Right", idleShadowID, idleCount, idleShdowOffset, idleShadowVector, 2, idleShdowInterval, true);
	playerShadowAnim->CreateAnimation(L"Shadow_Idle_Right_Down", idleShadowID, idleCount, idleShdowOffset, idleShadowVector, 3, idleShdowInterval, true);
	playerShadowAnim->CreateAnimation(L"Shadow_Idle_Down", idleShadowID, idleCount, idleShdowOffset, idleShadowVector, 4, idleShdowInterval, true);
	playerShadowAnim->CreateAnimation(L"Shadow_Idle_Left_Down", idleShadowID, idleCount, idleShdowOffset, idleShadowVector, 5, idleShdowInterval, true);
	playerShadowAnim->CreateAnimation(L"Shadow_Idle_Left", idleShadowID, idleCount, idleShdowOffset, idleShadowVector, 6, idleShdowInterval, true);
	playerShadowAnim->CreateAnimation(L"Shadow_Idle_Left_Up", idleShadowID, idleCount, idleShdowOffset, idleShadowVector, 7, idleShdowInterval, true);

	vector2Int runShadowVector = vector2Int(95, 34);
	wstring runShadowID = L"Shadow_Run";
	int runShadowCount = 22;
	vector2 runShdowOffset = vector2(-0.5f, 0.f);
	playerShadowAnim->CreateAnimation(L"Shadow_Run_Up", runShadowID, runShadowCount, runShdowOffset, runShadowVector, 0, runFI, true);
	playerShadowAnim->CreateAnimation(L"Shadow_Run_Up_Right", runShadowID, runShadowCount, runShdowOffset, runShadowVector, 1, runFI, true);
	playerShadowAnim->CreateAnimation(L"Shadow_Run_Right", runShadowID, runShadowCount, runShdowOffset, runShadowVector, 2, runFI, true);
	playerShadowAnim->CreateAnimation(L"Shadow_Run_Right_Down", runShadowID, runShadowCount, runShdowOffset, runShadowVector, 3, runFI, true);
	playerShadowAnim->CreateAnimation(L"Shadow_Run_Down", runShadowID, runShadowCount, runShdowOffset, runShadowVector, 4, runFI, true);
	playerShadowAnim->CreateAnimation(L"Shadow_Run_Left_Down", runShadowID, runShadowCount, runShdowOffset, runShadowVector, 5, runFI, true);
	playerShadowAnim->CreateAnimation(L"Shadow_Run_Left", runShadowID, runShadowCount, runShdowOffset, runShadowVector, 6, runFI, true);
	playerShadowAnim->CreateAnimation(L"Shadow_Run_Left_Up", runShadowID, runShadowCount, runShdowOffset, runShadowVector, 7, runFI, true);

	vector2Int miningShadowVector = vector2Int(146, 71);
	wstring miningShadowID = L"Shadow_Mining";
	int miningShadowCount = 26;
	vector2 miningShdowOffset = vector2(-0.15f, 0.2f);
	playerShadowAnim->CreateAnimation(L"Shadow_Mining_Up", miningShadowID, miningShadowCount, miningShdowOffset, miningShadowVector, 0, miningFI, true);
	playerShadowAnim->CreateAnimation(L"Shadow_Mining_Up_Right", miningShadowID, miningShadowCount, miningShdowOffset, miningShadowVector, 1, miningFI, true);
	playerShadowAnim->CreateAnimation(L"Shadow_Mining_Right", miningShadowID, miningShadowCount, miningShdowOffset, miningShadowVector, 2, miningFI, true);
	playerShadowAnim->CreateAnimation(L"Shadow_Mining_Right_Down", miningShadowID, miningShadowCount, miningShdowOffset, miningShadowVector, 3, miningFI, true);
	playerShadowAnim->CreateAnimation(L"Shadow_Mining_Down", miningShadowID, miningShadowCount, miningShdowOffset, miningShadowVector, 4, miningFI, true);
	playerShadowAnim->CreateAnimation(L"Shadow_Mining_Left_Down", miningShadowID, miningShadowCount, miningShdowOffset, miningShadowVector, 5, miningFI, true);
	playerShadowAnim->CreateAnimation(L"Shadow_Mining_Left", miningShadowID, miningShadowCount, miningShdowOffset, miningShadowVector, 6, miningFI, true);
	playerShadowAnim->CreateAnimation(L"Shadow_Mining_Left_Up", miningShadowID, miningShadowCount, miningShdowOffset, miningShadowVector, 7, miningFI, true);

	playerShadowAnim->SetAnimation(L"Shadow_Idle_Down");

	mainCam->SetTarget(player);

	// 채집 게이지
	CGameObject* collectGaugeBG = new CGameObject();
	collectGaugeBG->SetLayer(Layer::UI);
	collectGaugeBG->SetName(L"Collect Gauge BG");
	collectGaugeBG->AddComponent(new CImage(L""));
	CImage& cgi = *collectGaugeBG->GetComponent<CImage>();
	canvas->InsertUI(&cgi);
	cgi.getRect().SetSize(vector2(300, 15));
	cgi.getRect().SetAnchoredPos(vector2(0, -100));
	cgi.getRect().SetAnchorPreset(vector2(0, 1));
	cgi.SetColor(ColorValue::Black());

	m_pPlayer->SetCollectingGBObj(collectGaugeBG);

	CGameObject* collectGauge = new CGameObject();
	collectGauge->SetLayer(Layer::UI);
	collectGauge->SetName(L"Collect Gauge BG");
	collectGauge->AddComponent(new CImage(L"UI_GaugeBar_Yellow"));
	CImage& cg = *collectGauge->GetComponent<CImage>();
	cg.SetSpriteArea(vector2Int::zero(), vector2Int::one() * 16);
	cg.getRect().SetSize(cgi.getRect().getSize());
	canvas->InsertUI(&cg);
	cg.SetParent(&cgi);
	cg.SetFillAmount(vector2(0.f, 1.f));

	m_pPlayer->SetCollectingGauge(&cg);

	collectGaugeBG->SetActive(false);

	// 인벤토리창
	CGameObject* inventoryUI = new CGameObject();
	inventoryUI->SetName(L"InventoryUI");
	m_pInventoryUI = dynamic_cast<CUI*>(inventoryUI->AddComponent(new CImage(L"UI_Inventory")));
	canvas->InsertUI(inventoryUI->GetComponent<CImage>());
	inventoryUI->AddComponent(new CInventoryUI());
	m_pInventoryUI->getRect().SetSize(vector2(530.f * 1.2f, 290.f * 1.2f));
	m_pInventoryUI->getObj()->GetComponent<CImage>()->SetSpriteArea(vector2Int::zero(), vector2Int(530, 300));
	inventoryUI->SetActive(false);

	inventoryUI->Start();

	// 인벤토리
	CGameObject* inventory = AddObject(L"Inventory", Layer::Default);
	m_pPlayer->SetInventory(dynamic_cast<CInventory*>(inventory->AddComponent(new CInventory())));

	inventory->GetComponent<CInventory>()->SetIvenUI(inventoryUI->GetComponent<CInventoryUI>());
	inventoryUI->GetComponent<CInventoryUI>()->SetInven(m_pPlayer->getInventory());

	// 몬스터
	CGameObject* biter1 = AddObject(L"Biter1", Layer::Monster);
	biter1->AddComponent(new CMonster_Biter());
	biter1->getTransform().SetPosition(vector2(10, 30));
	biter1->getTransform().SetScale(vector2::one() * 2.f);
	m_vMonsterList.push_back(biter1);
	biter1->SetActive(false);

	CGameObject* biter2 = AddObject(L"Biter2", Layer::Monster);
	biter2->AddComponent(new CMonster_Biter());
	biter2->getTransform().SetPosition(vector2(15, 35));
	biter2->getTransform().SetScale(vector2::one() * 2.f);
	m_vMonsterList.push_back(biter2);
	biter2->SetActive(false);

	CGameObject* biter3 = AddObject(L"Biter3", Layer::Monster);
	biter3->AddComponent(new CMonster_Biter());
	biter3->getTransform().SetPosition(vector2(12, 40));
	biter3->getTransform().SetScale(vector2::one() * 2.f);
	m_vMonsterList.push_back(biter3);
	biter3->SetActive(false);

	// 엔딩
	CGameObject* endingObj = new CGameObject();
	endingObj->SetName(L"InventoryUI");
	m_pEndingPanel = dynamic_cast<CImage*>(endingObj->AddComponent(new CImage(L"UI_Ending")));
	m_pEndingPanel->getRect().SetSize(vector2(540.f, 450.f));
	m_pEndingPanel->SetSpriteArea(vector2Int::zero(), vector2Int(528, 455));
	canvas->InsertUI(m_pEndingPanel);
	endingObj->SetActive(false);

	CGameObject* endingTextObj = new CGameObject();
	endingTextObj->SetName(L"Ending UI Text");
	endingTextObj->SetLayer(Layer::UI);
	m_pEndingText = dynamic_cast<CText*>(endingTextObj->AddComponent(new CText()));
	m_pEndingText->SetText(L"승리!");
	m_pEndingText->getRect().SetSize(vector2(100.f, 50.f));
	m_pEndingText->getRect().SetAnchorPreset(vector2(-1, 1));
	m_pEndingText->getRect().SetChildPos(vector2(40.f, 30.f));
	m_pEndingText->getOption().fontSize = 26;
	m_pEndingText->getOption().bold = true;
	m_pEndingText->getOption().color = ColorValue(246, 222, 186);
	canvas->InsertUI(m_pEndingText);
	m_pEndingText->SetParent(m_pEndingPanel);
	endingTextObj->SetActive(false);
}

void CTestScene::Start()
{
	Start_();
}

void CTestScene::Update()
{
	Update_();

	CBeltManager::GetInstance().Update();

	KeyInput();
}

void CTestScene::FixedUpdate()
{
	FixedUpdate_();
}

void CTestScene::LateUpdate()
{
	LateUpdate_();
}

void CTestScene::Render(HDC _hDC)
{
	Render_(_hDC);	
}

void CTestScene::Release()
{
	Release_();

	for (TRAVERSAL_ITER(m_vTileArchive, it))
		Safe_Delete(*it);

	Safe_Delete(m_pDefaultMap);

	m_vTileArchive.clear();
	m_vMonsterList.clear();
}

void CTestScene::CloseInventoryUI()
{
	if (m_pInventoryUI->getObj()->isActive() && m_pInventoryUI->getObj()->isEnable())
		m_pInventoryUI->getObj()->SetActive(false);
}

void CTestScene::KeyInput()
{
	CCamera* cam = CSceneManager::GetInstance().getCurrentscene()->getMainCamera();

	if (CInput::GetInstance().GetMouseWheelValue() > 50)
	{
		cam->SetSize(cam->getSize() + 5.f * DELTA_TIME);

		if (cam->getSize() > m_v2ZoomLimit.y)
			cam->SetSize(m_v2ZoomLimit.y);
	}
	else if (CInput::GetInstance().GetMouseWheelValue() < -50)
	{
		cam->SetSize(cam->getSize() - 5.f * DELTA_TIME);

		if (cam->getSize() < m_v2ZoomLimit.x)
			cam->SetSize(m_v2ZoomLimit.x);
	}

	if (CInput::GetInstance().GetKeyDown(EIGHT))
		g_rectRencder = !g_rectRencder;

	if (CInput::GetInstance().GetKeyDown(TAB))
		m_pPlayer->getObj()->SetActive(!m_pPlayer->getObj()->isActive());

	if (CInput::GetInstance().GetKeyDown(E))
	{
		m_pPlayer->getInventory()->getInvenUI()->Reset();
		m_pPlayer->getInventory()->getInvenUI()->SetSideType(CInventoryUI::CreateItemTable);
		m_pInventoryUI->getObj()->SetActive(!m_pInventoryUI->getObj()->isActive());
	}

	if (CInput::GetInstance().GetKeyDown(ESCAPE))
	{
		if (m_pInventoryUI->getObj()->isActive())
			m_pInventoryUI->getObj()->SetActive(false);
	}

	if (CInput::GetInstance().GetKeyDown(THREE))
	{
		for (TRAVERSAL_ITER(m_vMonsterList, it))
			(*it)->SetActive(true);
	}

	if (CInput::GetInstance().GetKeyDown(FOUR))
	{
		m_pEndingPanel->getObj()->SetActive(true);
		m_pEndingText->getObj()->SetActive(true);
	}
}
