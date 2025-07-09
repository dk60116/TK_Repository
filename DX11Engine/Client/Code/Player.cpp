#include "cpch.h"
#include "Player.h"

CPlayer::CPlayer()
	: m_pMeshRenderer(nullptr)
	, m_pSkinnedMeshRenderer(nullptr)
	, m_pAnimator(nullptr)
{
	m_strName = L"Player";
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
	//CTexture* tex = CResources::GetInstance().LoadOnScene<CTexture>(L"Girl_Tex (Texture)");
	CMaterial* playerMat = CResources::GetInstance().LoadOnGame<CMaterial>(L"UnlitMaterial (Material)");
	playerMat->Set_Texture(tex, 0);

	//CMeshBuffer* mb = CResources::GetInstance().LoadOnScene<CMeshBuffer>(L"Link_Model (MeshBuffer)");
	CMeshBuffer* mb = CResources::GetInstance().LoadOnScene<CMeshBuffer>(L"Girl_Model (MeshBuffer)");

	//m_pMeshRenderer = m_pGameObject->AddComponent<CMeshRenderer>();
	//mb->Set_Scalefactor(0.01f);
	//m_pMeshRenderer->Get_MeshFilter()->Set_MeshBuffer(mb);
	//m_pMeshRenderer->Set_Material(playerMat);

	CSkinnedMeshBuffer* smb = CResources::GetInstance().LoadOnScene<CSkinnedMeshBuffer>(L"Link_Model (SkinnedMeshBuffer)");
	m_pSkinnedMeshRenderer = m_pGameObject->AddComponent<CSkinnedMeshRenderer>();
	m_pSkinnedMeshRenderer->Set_Mesh(smb);
	m_pSkinnedMeshRenderer->Set_Material(playerMat);

	m_pAnimator = m_pGameObject->AddComponent<CAnimator>();
	m_pAnimator->Add_Animation(L"Idle", CResources::GetInstance().LoadOnScene<CAnimation>(L"Link_Idle (Animation)"));
	m_pAnimator->Add_Animation(L"Run", CResources::GetInstance().LoadOnScene<CAnimation>(L"Link_Run (Animation)"));

	m_pAnimator->SetLoop(true);
	//m_pAnimator->Set_PlaybackSpeed(0.1f);
	//m_pAnimator->Play(L"Run");

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
	if (CInput::GetInstance().GetKeyDown(N))
	{
		if (m_pAnimator)
			m_pAnimator->Play(L"Idle", 0.2f);
	}

	if (CInput::GetInstance().GetKeyDown(M))
	{
		if (m_pAnimator)
			m_pAnimator->Play(L"Run", 0.1f);
	}

	if (CInput::GetInstance().GetKeyDown(X))
	{
		if (m_pAnimator)
			m_pAnimator->Stop();
	}

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
	if (CInput::GetInstance().GetKey(R))
	{
		Get_Transform()->Add_EulerAnglesZ(-45.f * DELTA_TIME);
	}
	if (CInput::GetInstance().GetKey(T))
	{
		Get_Transform()->Add_EulerAnglesZ(45.f * DELTA_TIME);
	}
}

void CPlayer::OnDestroy()
{
}
