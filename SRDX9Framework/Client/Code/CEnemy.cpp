#include "pch.h"
#include "CEnemy.h"

CEnemy::CEnemy()
	: m_pRenderer(nullptr)
	, m_fMoveSpeed(0.5f)
	, m_fRotaionSpeed(45.f)
	, m_pTarget(nullptr)
	, m_bInit(false)
{
}

CEnemy::~CEnemy()
{
}

void CEnemy::Awake()
{
	CComponent::Awake();

	getTransform().SetPosition(2.f, 2.f, 0.f);
	getTransform().SetLocalScale(1.5f, 1.5f, 1.f);
	m_pRenderer = m_pGameObject->AddComponent<CSpriteRenderer>();
	auto tex = CResources::GetInstance().getResource<CTexture>(L"Enemy").get();
	if (tex)
		m_pRenderer->SetTexture(tex);
	m_pRenderer->SetTexture(tex);
	//m_pRenderer->SetTintColor(ColorValue::red());

	m_bInit = true;
}

void CEnemy::Start()
{
	CComponent::Start();
}

void CEnemy::Update()
{
	CComponent::Update();

	CTransform& t = getTransform();

	if (m_pTarget)
	{
		float distance = vector3::Distance(getTransform().getPosition(), m_pTarget->getPosition());

		if (distance > 0.1f)
		{
			getTransform().LookAt(m_pTarget->getPosition(), vector3::up());
			getTransform().AddPosition(getTransform().getDirections().up * DELTA_TIME * m_fMoveSpeed);
		}
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
