#include "cpch.h"
#include "Player.h"

CPlayer::CPlayer()
	: m_pMeshRenderer(nullptr)
	, m_pSkinnedMeshRenderer(nullptr)
	, m_pAnimator(nullptr)
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

	//m_pMeshRenderer = m_pGameObject->AddComponent<CMeshRenderer>();
	//m_pMeshRenderer->Get_MeshFilter()->CreateMeshBuffer(L"Object", "../Assets/Link_Idle.fbx", 0.01f);
	//m_pMeshRenderer->Set_Material(playerMat);

	m_pSkinnedMeshRenderer = m_pGameObject->AddComponent<CSkinnedMeshRenderer>();
	m_pSkinnedMeshRenderer->CreateSkinnedMeshBuffer("../Assets/Animation_Archery_Shot_1_withSkin.fbx", 0.01f);
	m_pSkinnedMeshRenderer->Set_Material(playerMat);

	CAnimation* anim_Idle = CResources::GetInstance().CreateResource<CAnimation>(L"../Assets/Animation_Archery_Shot_1_withSkin.fbx");

	CAnimator* m_pAnimator = m_pGameObject->AddComponent<CAnimator>();
	m_pAnimator->Add_Animation(L"Idle", anim_Idle);

	m_pAnimator->Play(L"Idle");

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
