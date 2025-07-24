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

	m_pGameObject->CreateSkinnedMeshHierachy(CResources::GetInstance().LoadSkinnedMeshBuffersOnScene(L"Link_Model (MeshBuffer)"), CResources::GetInstance().LoadSkinnedBonesOnScene(L"Link_Model (MeshBuffer)"));

	m_pGameObject->Get_Transform()->Get_Child(0)->Get_GameObject()->GetComponent<CSkinnedMeshRenderer>()->Get_Material()->Set_Texture(tex, 0);

	m_pAnimator = m_pGameObject->AddComponent<CAnimator>();
	m_pAnimator->Add_Animation(L"Idle", CResources::GetInstance().LoadOnScene<CAnimationClip>(L"Link_Idle (Animation)"));
	m_pAnimator->Add_Animation(L"Run", CResources::GetInstance().LoadOnScene<CAnimationClip>(L"Link_Run (Animation)"));

	m_pAnimator->SetLoop(true);

	Get_Transform()->Get_Child(0)->Set_LocalScale(0.01f);
	Get_Transform()->Get_Child(1)->Set_LocalScale(0.01f);
	Get_Transform()->Get_Child(0)->Set_LocalEulerAnglesY(180.f);
	Get_Transform()->Get_Child(1)->Set_LocalEulerAnglesY(180.f);

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
	if (CInput::GetInstance().GetKeyDown_Editor(N))
	{
		if (m_pAnimator)
			m_pAnimator->Play(L"Idle", 0.2f);
	}

	if (CInput::GetInstance().GetKeyDown_Editor(M))
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
		Get_Transform()->Add_Position(Get_Transform()->Get_Directions().left * DELTA_TIME);
	}
	if (CInput::GetInstance().GetKey(D))
	{
		Get_Transform()->Add_Position(Get_Transform()->Get_Directions().right * DELTA_TIME);
	}
}

void CPlayer::OnDestroy()
{
}
