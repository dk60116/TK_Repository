#include "pch.h"
#include "CPlayer.h"

CPlayer::CPlayer()
	: m_pMeshRenderer(nullptr)
	, m_pAudioSource(nullptr)
	, m_fMoveSpeed(1.f)
	, m_fRotaionSpeed(90.f)
{
	m_strName = L"Player";
}

CPlayer::~CPlayer()
{
	OnDestroy();
}

CPlayer* CPlayer::Create()
{
	return new CPlayer();
}

void CPlayer::Awake()
{
	CComponent::Awake();

	getTransform().SetLocalScale(1.5f);

	m_pMeshRenderer = m_pGameObject->AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMeshFilterType(CMesh::SPHERE);

	m_pAudioSource = m_pGameObject->AddComponent<CAudioSource>();

	auto bgm = CResources::GetInstance().getResource<CAudioClip>(L"beyond-factory-outskirts").get();
	if (bgm)
		m_pAudioSource->SetClip(bgm);

	m_pAudioSource->Play();
}

void CPlayer::Start()
{
}

void CPlayer::Update()
{
	CComponent::Update();

	KeyInput();
}

void CPlayer::FixedUpdate()
{
}

void CPlayer::LateUpdate()
{
}

void CPlayer::Render()
{
}

void CPlayer::OnEnable()
{
}

void CPlayer::OnDisable()
{
}

void CPlayer::OnDestroy()
{
	CComponent::OnDestroy();
}

void CPlayer::KeyInput()
{
	if (CInput::GetInstance().GetKey(W))
	{
		getTransform().AddPosition(getTransform().getDirections().up * DELTA_TIME * m_fMoveSpeed);
	}
	if (CInput::GetInstance().GetKey(S))
	{
		getTransform().AddPosition(getTransform().getDirections().down * DELTA_TIME * m_fMoveSpeed);
	}
	if (CInput::GetInstance().GetKey(A))
	{
		getTransform().AddLocalZAxis(DELTA_TIME * m_fRotaionSpeed);
	}
	if (CInput::GetInstance().GetKey(D))
	{
		getTransform().AddLocalZAxis(DELTA_TIME * -m_fRotaionSpeed);
	}
	if (CInput::GetInstance().GetKey(Q))
	{
		getTransform().AddLocalXAxis(DELTA_TIME * -m_fRotaionSpeed);
	}
	if (CInput::GetInstance().GetKey(E))
	{
		getTransform().AddLocalXAxis(DELTA_TIME * m_fRotaionSpeed);
	}
}
