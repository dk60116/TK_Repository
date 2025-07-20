#include "cpch.h"
#include "MainScene.h"
#include "Player.h"
#include "UI.h"

CMainScene::CMainScene()
	: CScene{}
	, m_pMainCamera(nullptr)
	, m_pDirLight(nullptr)
	, m_pCanvas(nullptr)
	, m_pPlayer(nullptr)
	, m_pImage2(nullptr)
{
}

CMainScene::~CMainScene()
{
}

HRESULT CMainScene::Initialize()
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

	//boxObj->Get_Transform()->SetParent(playerObj->Get_Transform());
	boxObj->Get_Transform()->Set_LocalPosition(vector3(0.f, -3.f, 0.f));

	box1Obj->Get_Transform()->Set_Position(vector3(0.f, 0.f, 0.f));
	box1Obj->Get_Transform()->SetParent(boxObj->Get_Transform());

	box2Obj->Get_Transform()->SetParent(box1Obj->Get_Transform());
	box2Obj->Get_Transform()->Set_LocalPosition(vector3(-1.f, 0.f, 0.f));

	m_pMainCamera->Get_Transform()->Set_Position(0.f, 5.f, -5.f);
	m_pMainCamera->Get_Transform()->Set_EulerAnglesX(20.f);
	
	m_pPlayer->Get_Transform()->Get_Child()->Set_LocalScale(0.01f);

	CGameObject* quadObject = Add_GameObject(L"Rect");
	CMeshRenderer* quadRender = quadObject->AddComponent<CMeshRenderer>();
	quadRender->Get_MeshFilter()->Set_MeshBuffer(CResources::GetInstance().LoadOnGame<CMeshBuffer>(L"Quad (Mesh Buffer)"));
	quadRender->Set_Material(boxMat);

	CGameObject* ImageObject = Add_GameObject(L"Image");
	CImage* image = ImageObject->AddComponent<CImage>();

	image->SetTexture(CResources::GetInstance().LoadOnScene<CTexture>(L"Main_BG (Texture)"));

	CGameObject* ImageObject2 = Add_GameObject(L"Image2");
	m_pImage2 = ImageObject2->AddComponent<CImage>();
	m_pImage2->SetTexture(CResources::GetInstance().LoadOnScene<CTexture>(L"Main_Logo (Texture)"));

	m_pImage2->SetFillAmount(1.f);

	ImageObject->Get_Transform()->SetParent(canvasObj->Get_Transform());
	image->Get_RectTransform()->Set_WidthHeight(1280, 720);

	ImageObject2->Get_Transform()->SetParent(image->Get_Transform());
	m_pImage2->Get_RectTransform()->Set_WidthHeight(200, 200);

	//m_pMainCamera->Set_ViewMode(CCamera::ViewMode::ORTHOGRAPHIC);

	CGameObject* templeObj = Add_GameObject(L"Temple");
 	CMeshRenderer* templeRenderer = templeObj->AddComponent<CMeshRenderer>();
	
	templeObj->CreateMeshHierachy(CResources::GetInstance().LoadMeshBuffersOnScene(L"Temple_Model (MeshBuffer)"));

	templeRenderer->Get_Material()->Set_FloatValue(L"Smoothness", 0.5f);
	templeRenderer->Get_Transform()->Get_Transform()->Set_LocalScale(0.01f);

	CGameObject* terrainObj = Add_GameObject(L"Terrain");
	CMeshRenderer* terrainBuffer = terrainObj->AddComponent<CMeshRenderer>();
	terrainBuffer->Get_MeshFilter()->Set_MeshBuffer(CResources::GetInstance().LoadOnScene<CMeshBuffer>(L"Sample_Terrain (Terrain MeshBuffer)"));
	terrainBuffer->Get_Material()->Set_Texture(CResources::GetInstance().LoadOnScene<CTexture>(L"Terrain_MainTex (Texture)")); 

	return S_OK;
}

void CMainScene::Update()
{
	__super::Update();

	if (CInput::GetInstance().GetKeyDown(Alpha1))
		CSceneManager::GetInstance().LoadScene(L"Main Scene");

	_float fill = m_pImage2->GetFillAmount();

	if (CInput::GetInstance().GetKey(O))
	{
		m_pImage2->SetFillAmount(fill - DELTA_TIME);
		CDebug::Log(m_pImage2->GetFillAmount());
	}
	if (CInput::GetInstance().GetKey(P))
	{
		m_pImage2->SetFillAmount(fill + DELTA_TIME);
		CDebug::Log(m_pImage2->GetFillAmount());
	}
}
