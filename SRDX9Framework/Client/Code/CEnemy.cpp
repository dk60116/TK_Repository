#include "pch.h"
#include "CEnemy.h"
#include "CInput.h"

CEnemy::CEnemy()
	: m_pRenderer(nullptr)
	, m_fMoveSpeed(0.5f)
	, m_fRotaionSpeed(45.f)
	, m_pTarget(nullptr)
{
}

CEnemy::~CEnemy()
{
}

void CEnemy::Awake()
{
	CComponent::Awake();

	m_pRenderer = m_pGameObject->AddComponent<CSpriteRenderer>();
	m_pRenderer->SetTintColor(ColorValue::red());
}

void CEnemy::Start()
{
}

void CEnemy::Update()
{
	CComponent::Update();

	float distance = vector3::Distance(getTransform().getPosition(), m_pTarget->getPosition());

	if (m_pTarget && distance > 0.1f)
	{
		getTransform().LookAt(*m_pTarget);
		getTransform().AddPosition(getTransform().getDirections().forward * DELTA_TIME * m_fMoveSpeed);
	}
}

void CEnemy::FixedUpdate()
{
}

void CEnemy::LateUpdate()
{
}

void CEnemy::Render()
{
}

void CEnemy::OnEnable()
{
}

void CEnemy::OnDisable()
{
}

void CEnemy::OnDestroy()
{
	CComponent::OnDestroy();
}
