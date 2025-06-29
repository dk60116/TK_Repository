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

	CGameObject* playerObj = Add_GameObject(L"Player");
	m_pPlayer = playerObj->AddComponent<CPlayer>();

	CGameObject* cameraObject = Add_GameObject(L"Main Camera");
	m_pMainCamera = cameraObject->AddComponent<CCamera>();

	CTexture* tex = CResources::GetInstance().CreateResource<CTexture>(L"../Assets/texture_0.png");
	CMaterial* boxMat = CMaterial::Create();
	boxMat->Set_Texture(tex, 0);
	CGameObject* boxObj = Add_GameObject(L"Box");
	CMeshRenderer* boxRenderer = boxObj->AddComponent<CMeshRenderer>();
	boxRenderer->Get_MeshFilter()->CreateMeshBuffer(L"Cube");
	boxRenderer->Set_Material(boxMat);

	boxObj->Get_Transfrom()->Set_Parent(playerObj->Get_Transfrom());

	m_pMainCamera->Get_Transform()->Set_Position(0.f, 0.f, -5.f);

	return S_OK;
}

void CMainScene::Update()
{
	__super::Update();
}
