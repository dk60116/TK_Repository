#include "pch.h"
#include "CPlayer.h"
#include "CInput.h"

CPlayer::CPlayer()
	: m_pRenderer(nullptr)
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

	m_pRenderer = m_pGameObject->AddComponent<CSpriteRenderer>();
	m_pRenderer->SetTintColor(ColorValue::green());
}

void CPlayer::Start()
{
}

void CPlayer::Update()
{
	vector3 pos = getTransform().getPosition();

	char buffer[128];
	sprintf_s(buffer, "Position: X = %.2f, Y = %.2f, Z = %.2f", pos.x, pos.y, pos.z);

	HWND hWnd = ::GetActiveWindow();
	//SetWindowTextA(hWnd, buffer);

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
		getTransform().AddEulerAngles(vector3::forward() * DELTA_TIME * m_fRotaionSpeed);
	}
	if (CInput::GetInstance().GetKey(D))
	{
		getTransform().AddEulerAngles(vector3::backward() * DELTA_TIME * m_fRotaionSpeed);
	}
	if (CInput::GetInstance().GetKey(Q))
	{
		getTransform().AddEulerAngles(vector3::up() * DELTA_TIME * m_fRotaionSpeed);
	}
	if (CInput::GetInstance().GetKey(E))
	{
		getTransform().AddEulerAngles(vector3::down() * DELTA_TIME * m_fRotaionSpeed);
	}
}
