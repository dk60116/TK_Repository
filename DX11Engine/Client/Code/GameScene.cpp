#include "cpch.h"
#include "GameScene.h"
#include "Player.h"

CGameScene::CGameScene()
	: m_pMainCamera(nullptr)
	, m_pDirLight(nullptr)
	, m_pCanvas(nullptr)
	, m_pPlayer(nullptr)
{
}

CGameScene::~CGameScene()
{
}

HRESULT CGameScene::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	CGameObject* cameraObject = Add_GameObject(L"Main Camera");
	m_pMainCamera = cameraObject->AddComponent<CCamera>();

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

	CTexture* linkTex = CResources::GetInstance().LoadOnScene<CTexture>(L"Link_Texture (Texture)");
	CMaterial* boxMat = CResources::GetInstance().CloneOnGame<CMaterial>(L"LitMaterial (Material)");
	boxMat->Set_Texture(linkTex, 0);

	CGameObject* boxObj = Add_GameObject(L"Box");
	CMeshRenderer* boxRenderer = boxObj->AddComponent<CMeshRenderer>();
	boxRenderer->Get_MeshFilter()->Set_MeshBuffer(CResources::GetInstance().LoadOnGame<CMeshBuffer>(L"Cube (Mesh Buffer)"));
	boxRenderer->Set_Material(boxMat);

	CGameObject* box2Obj = Add_GameObject(L"Box2");
	CMeshRenderer* box2Renderer = box2Obj->AddComponent<CMeshRenderer>();
	box2Renderer->Get_MeshFilter()->Set_MeshBuffer(CResources::GetInstance().LoadOnGame<CMeshBuffer>(L"Cube (Mesh Buffer)"));
	box2Renderer->Set_Material(boxMat);

	CGameObject* box1Obj = Add_GameObject(L"Box1");
	CMeshRenderer* box1Renderer = box1Obj->AddComponent<CMeshRenderer>();
	box1Renderer->Get_MeshFilter()->Set_MeshBuffer(CResources::GetInstance().LoadOnGame<CMeshBuffer>(L"Cube (Mesh Buffer)"));
	box1Renderer->Set_Material(boxMat);

	boxObj->Get_Transform()->Set_LocalPosition(vector3(0.f, -3.f, 0.f));

	box1Obj->Get_Transform()->Set_Position(vector3(0.f, 0.f, 0.f));
	box1Obj->Get_Transform()->SetParent(boxObj->Get_Transform());

	box2Obj->Get_Transform()->SetParent(box1Obj->Get_Transform());
	box2Obj->Get_Transform()->Set_LocalPosition(vector3(-1.f, 0.f, 0.f));

	m_pMainCamera->Get_Transform()->Set_Position(0.f, 5.f, -5.f);
	m_pMainCamera->Get_Transform()->Set_EulerAnglesX(20.f);

	CGameObject* quadObject = Add_GameObject(L"Rect");
	CMeshRenderer* quadRender = quadObject->AddComponent<CMeshRenderer>();
	quadRender->Get_MeshFilter()->Set_MeshBuffer(CResources::GetInstance().LoadOnGame<CMeshBuffer>(L"Quad (Mesh Buffer)"));
	quadRender->Set_Material(boxMat);

	//CGameObject* templeObj = Add_GameObject(L"Temple");
	//	CMeshRenderer* templeRenderer = templeObj->AddComponent<CMeshRenderer>();
	//
	//templeObj->CreateMeshHierachy(CResources::GetInstance().LoadMeshBuffersOnScene(L"Temple_Model (MeshBuffer)"));

	//CGameObject* castleObj = Add_GameObject(L"Castle");
	//castleObj->CreateMeshHierachy(CResources::GetInstance().LoadMeshBuffersOnScene(L"Castle_Model (MeshBuffer)"));

	CGameObject* vahMedoh_BodyObj = Add_GameObject(L"VahMedoh_Body");
	vahMedoh_BodyObj->CreateMeshHierachy(CResources::GetInstance().LoadMeshBuffersOnScene(L"VM (MeshBuffer)"));

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
}
