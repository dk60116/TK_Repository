#include "cpch.h"
#include "MainScene.h"
#include "Player.h"

CMainScene::CMainScene()
	: CScene{}
	, m_pMainCamera(nullptr)
	, m_pPlayer(nullptr)
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

	CGameObject* playerObj = Add_GameObject(L"Player");
	m_pPlayer = playerObj->AddComponent<CPlayer>();

	CMeshBuffer* mb = CResources::GetInstance().CreateResource<CMeshBuffer>(L"Cube", L"Cube", nullptr);

	CTexture* tex = CResources::GetInstance().LoadOnScene<CTexture>(L"TestTexture (Texture)");
	CMaterial* boxMat = CMaterial::Create();
	boxMat->Set_Texture(tex, 0);

	boxObj = Add_GameObject(L"Box");
	CMeshRenderer* boxRenderer = boxObj->AddComponent<CMeshRenderer>();
	boxRenderer->Get_MeshFilter()->Set_MeshBuffer(mb);
	boxRenderer->Set_Material(boxMat);

	CGameObject* box2Obj = Add_GameObject(L"Box2");
	CMeshRenderer* box2Renderer = box2Obj->AddComponent<CMeshRenderer>();
	box2Renderer->Get_MeshFilter()->Set_MeshBuffer(mb);
	box2Renderer->Set_Material(boxMat);

	CGameObject* box1Obj = Add_GameObject(L"Box1");
	CMeshRenderer* box1Renderer = box1Obj->AddComponent<CMeshRenderer>();
	box1Renderer->Get_MeshFilter()->Set_MeshBuffer(mb);
	box1Renderer->Set_Material(boxMat);

	boxObj->Get_Transform()->Set_Parent(playerObj->Get_Transform());
	boxObj->Get_Transform()->Set_LocalPosition(vector3(0.f, - 1.f, 0.f));

	box1Obj->Get_Transform()->Set_Parent(boxObj->Get_Transform());
	box1Obj->Get_Transform()->Set_LocalPosition(vector3(0.f, -1.f, 0.f));

	box2Obj->Get_Transform()->Set_Parent(box1Obj->Get_Transform());
	box2Obj->Get_Transform()->Set_LocalPosition(vector3(-1.f, 0.f, 0.f));

	m_pMainCamera->Get_Transform()->Set_Position(0.f, 0.f, -5.f);
	m_pPlayer->Get_Transform()->Get_Child()->Set_LocalScale(0.01f);
	CSceneManager::GetInstance().Get_CrtScene()->Get_EditorCamera()->Get_Transform()->Set_PositionZ(-5.f);

	return S_OK;
}

void CMainScene::Update()
{
	__super::Update();

	if (CInput::GetInstance().GetKeyDown(Alpha1))
		CSceneManager::GetInstance().LoadScene(L"Main Scene");

	if (CInput::GetInstance().GetKeyDown(Alpha4))
		boxObj->Get_Transform()->Set_Parent(nullptr);
}
