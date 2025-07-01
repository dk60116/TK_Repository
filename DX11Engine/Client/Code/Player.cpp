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
	__super::Initialize();

	CTexture* tex = CResources::GetInstance().LoadOnScene<CTexture>(L"Link_Texture (Texture)");
	CMaterial* playerMat = CMaterial::Create();
	playerMat->Set_Texture(tex, 0);

	CMeshBuffer* mb = CResources::GetInstance().LoadOnScene<CMeshBuffer>(L"Link_Model (MeshBuffer)");

	//m_pMeshRenderer = m_pGameObject->AddComponent<CMeshRenderer>();
	//mb->Set_Scalefactor(0.01f);
	//m_pMeshRenderer->Get_MeshFilter()->Set_MeshBuffer(mb);
	//m_pMeshRenderer->Set_Material(playerMat);

	CSkinnedMeshBuffer* smb = CResources::GetInstance().LoadOnScene<CSkinnedMeshBuffer>(L"Link_Model (SkinnedMeshBuffer)");
	m_pSkinnedMeshRenderer = m_pGameObject->AddComponent<CSkinnedMeshRenderer>();
	m_pSkinnedMeshRenderer->Set_Mesh(smb);
	m_pSkinnedMeshRenderer->Set_Material(playerMat);

	//CAnimation* anim_Idle = CResources::GetInstance().CreateResource<CAnimation>(L"../Assets/Animation_Archery_Shot_1_withSkin.fbx");

	//CAnimator* m_pAnimator = m_pGameObject->AddComponent<CAnimator>();
	//m_pAnimator->Add_Animation(L"Idle", anim_Idle);

	//m_pAnimator->Play(L"Idle");

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
	if (CInput::GetInstance().GetKey(W))
	{
		Get_Transform()->Add_Position(Get_Transform()->Get_Directions().forward * DELTA_TIME);
	}
	if (CInput::GetInstance().GetKey(S))
	{
		Get_Transform()->Add_Position(Get_Transform()->Get_Directions().back * DELTA_TIME);
	}
	if (CInput::GetInstance().GetKey(A))
	{
		Get_Transform()->Add_EulerAnglesY(-45.f * DELTA_TIME);
	}
	if (CInput::GetInstance().GetKey(D))
	{
		Get_Transform()->Add_EulerAnglesY(45.f * DELTA_TIME);
	}
	if (CInput::GetInstance().GetKey(E))
	{
		Get_Transform()->Add_EulerAnglesX(45.f * DELTA_TIME);
	}
	if (CInput::GetInstance().GetKey(Q))
	{
		Get_Transform()->Add_EulerAnglesX(-45.f * DELTA_TIME);
	}
}

void CPlayer::OnDestroy()
{
}
