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
	CTexture* tex = CResources::GetInstance().CreateResource<CTexture>(L"../Assets/texture_0.png");
	CMaterial* playerMat = CMaterial::Create();
	playerMat->Set_Texture(tex, 0);
	m_pMeshRenderer = m_pGameObject->AddComponent<CSkinnedMeshRenderer>();
	m_pMeshRenderer->CreateSkinnedMeshBuffer("../Assets/Link_Idle.fbx", 0.01f);
	m_pMeshRenderer->Set_Material(playerMat);

	CAnimation* an = CResources::GetInstance().CreateResource<CAnimation>(L"../Assets/Link_Idle.fbx");

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
