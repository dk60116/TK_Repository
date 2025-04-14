#include "pch.h"
#include "CPlayer.h"

CPlayer::CPlayer()
	: m_pRenderer(nullptr)
	, m_pMeshRenderer(nullptr)
	, m_fMoveSpeed(1.f)
	, m_fRotaionSpeed(90.f)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Awake()
{
	CComponent::Awake();

	m_pMeshRenderer = m_pGameObject->AddComponent<CMeshRenderer>();
	CMeshFilter* mf = m_pGameObject->AddComponent<CMeshFilter>();
	m_pMeshRenderer->SetMeshFilter(mf);
	mf->SetMesh(CMesh::CUBE);

	//m_pRenderer = m_pGameObject->AddComponent<CSpriteRenderer>();
	//auto tex = CResources::GetInstance().getResource<CTexture>(L"Player").get();
	//if (tex)
		//m_pRenderer->SetTexture(tex);
	//m_pRenderer->SetTintColor(ColorValue::green());
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
}

void CPlayer::KeyInput()
{
	if (CInput::GetInstance().GetKey(ONE))
		m_pRenderer->SetTintColor(ColorValue::red());

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
