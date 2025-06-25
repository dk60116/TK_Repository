#include "cpch.h"
#include "Player.h"

CPlayer::CPlayer()
	: m_pMeshRenderer(nullptr)
{
}

CPlayer::~CPlayer()
{
}

CPlayer* CPlayer::Create()
{
	return new CPlayer();
}

HRESULT CPlayer::Initialize()
{
	CMaterial* playerMat = CMaterial::Create();

	m_pMeshRenderer = m_pGameObject->AddComponent<CMeshRenderer>();
	m_pMeshRenderer->Get_MeshFilter()->CreateMeshBuffer(L"Object", "../Assets/Animation_Idle_02_withSkin.fbx");
	m_pMeshRenderer->Set_Material(playerMat);

	return S_OK;
}

void CPlayer::Awake()
{
}

void CPlayer::Start()
{
}

void CPlayer::Update()
{
}

void CPlayer::OnDestroy()
{
}
