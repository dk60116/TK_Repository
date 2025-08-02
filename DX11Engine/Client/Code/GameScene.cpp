#include "cpch.h"
#include "GameScene.h"
#include "Player.h"
#include "PlayerCamera.h"
#include "Wolf.h"

CGameScene::CGameScene()
	: m_pPlayerCamera(nullptr)
	, m_pDirLight(nullptr)
	, m_pCanvas(nullptr)
	, m_pPlayer(nullptr)
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
	lightObject->Get_Transform()->Set_EulerAnglesX(90.f);

	CGameObject* lightObject2 = Add_GameObject(L"Point Light");
	CLight* pointLight = lightObject2->AddComponent<CLight>();
	pointLight->Set_Type(CLight::Type::point);

	CGameObject* canvasObj = Add_GameObject(L"Canvas");
	m_pCanvas = canvasObj->AddComponent<CCanvas>();

	CGameObject* playerObj = Add_GameObject(L"Player");
	m_pPlayer = playerObj->AddComponent<CPlayer>();

	//CGameObject* templeObj = Add_GameObject(L"Temple");
	//	CMeshRenderer* templeRenderer = templeObj->AddComponent<CMeshRenderer>();
	//
	//templeObj->CreateMeshHierachy(CResources::GetInstance().LoadMeshBuffersOnScene(L"Temple_Model (MeshBuffer)"));

	//CGameObject* castleObj = Add_GameObject(L"Castle");
	//castleObj->CreateMeshHierachy(CResources::GetInstance().LoadMeshBuffersOnScene(L"Castle_Model (MeshBuffer)"));

	CGameObject* vahMedoh_BodyObj = Add_GameObject(L"VahMedoh_Body");
	vahMedoh_BodyObj->CreateMeshHierachy(CResources::GetInstance().LoadMeshBuffersOnScene(L"VahMedoh (MeshBuffer)"));

	CGameObject* woolfObject = Add_GameObject(L"Wolf");
	CWolf* woolf = woolfObject->AddComponent<CWolf>();
	m_vMonsters.push_back(woolf);

	if (woolfObject)
	{
		for (_uint i = 0; i < 2; ++i)
		{
			CGameObject* cloneWolf = CGameObject::Instantiate(woolfObject);
			m_vMonsters.push_back(cloneWolf->GetComponent<CWolf>());
		}
	}

	return S_OK;
}

void CGameScene::Awake()
{
	__super::Awake();
}

void CGameScene::Update()
{
	__super::Update();

	if (CInput::GetInstance().GetKeyDown(Alpha1))
	{
		CSceneManager::GetInstance().LoadScene(L"Main Scene");
	}

	if (CInput::GetInstance().GetKeyDown(K))
	{
		m_vMonsters[0]->Get_Animator()->Play(L"Idle");
	}
}

void CGameScene::SceneRelease()
{
	__super::SceneRelease();

	m_vMonsters.clear();
}
