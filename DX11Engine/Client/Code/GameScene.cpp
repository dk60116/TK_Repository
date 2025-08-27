#include "cpch.h"
#include "GameScene.h"
#include "Player.h"
#include "PlayerCamera.h"
#include "PlayerHUD.h"
#include "Dungeon.h"
#include "Wolf.h"
#include "Troll.h"

CGameScene::CGameScene()
	: m_pPlayerCamera(nullptr)
	, m_pDirLight(nullptr)
	, m_pPlayer(nullptr)
	, m_pHUD(nullptr)
	, m_pDungeon(nullptr)
	, m_vMonsters({})
{
}

CGameScene::~CGameScene()
{
}

HRESULT CGameScene::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	CGameObject* cameraObject = Add_GameObject(L"Player Camera");
	m_pPlayerCamera = cameraObject->AddComponent<CPlayerCamera>();

	CGameObject* lightObject = Add_GameObject(L"Directional Light");
	m_pDirLight = lightObject->AddComponent<CLight>();
	m_pDirLight->Get_Transform()->Set_EulerAngles(25.f, 20.f, 0.f);

	CGameObject* lightObject2 = Add_GameObject(L"Point Light");
	CLight* pointLight = lightObject2->AddComponent<CLight>();
	pointLight->Set_Type(CLight::Type::point);

	CGameObject* hudObject = Add_GameObject(L"Player HUD");
	m_pHUD = hudObject->AddComponent<CPlayerHUD>();

	CGameObject* playerObj = Add_GameObject(L"Player");
	m_pPlayer = playerObj->AddComponent<CPlayer>();

	//CGameObject* vahMedoh_BodyObj = Add_GameObject(L"VahMedoh_Body");
	//vahMedoh_BodyObj->CreateMeshHierachy(CResources::LoadMeshBuffersOnScene(L"VahMedoh (MeshBuffer)"), 0.01f, CGameObject::navigationStatic);
	
	CGameObject* dungeonObj = Add_GameObject(L"Dungeon");
	m_pDungeon = dungeonObj->AddComponent<CDungeon>();

	CGameObject* trollObject = Add_GameObject(L"Troll");
	CTroll* troll = trollObject->AddComponent<CTroll>();
	//m_vMonsters.push_back(lizard);
	trollObject->SetActive(false);

	CGameObject* wolfObject = Add_GameObject(L"Wolf");
	CWolf* wolf = wolfObject->AddComponent<CWolf>();
	m_vMonsters.push_back(wolf);
	wolfObject->SetActive(false);

	return S_OK;
}

void CGameScene::Awake()
{
	__super::Awake();

	CGameObject* wolfObject = nullptr;

	if (!m_vMonsters.empty() && m_vMonsters[0])
	{
		wolfObject = m_vMonsters[0]->Get_GameObject();

		for (_uint i = 0; i < 3; ++i)
		{
			CGameObject* cloneWolf = CGameObject::Instantiate(wolfObject);
			m_vMonsters.push_back(cloneWolf->GetComponent<CWolf>());
			m_vMonsters.back()->Get_GameObject()->SetActive(true);
			m_vMonsters.back()->Get_Transform()->Set_PositionX(20.f + (i) * 1.5f);
		}
	}

	m_pPlayer->Set_Focus(m_vMonsters[0]->Get_Transform());
}

void CGameScene::Update()
{
	__super::Update();

	if (CInput::GetKeyDown(Alpha0))
	{
		CSceneManager::LoadScene(L"Main Scene");
	}
}

void CGameScene::SceneRelease()
{
	__super::SceneRelease();

	m_vMonsters.clear();
}
