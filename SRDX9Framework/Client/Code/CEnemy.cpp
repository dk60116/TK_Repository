#include "pch.h"
#include "CEnemy.h"

CEnemy::CEnemy()
	: m_pRenderer(nullptr)
	, m_fMoveSpeed(0.5f)
	, m_fRotaionSpeed(45.f)
	, m_pTarget(nullptr)
	, m_bInit(false)
{
	m_strName = L"Enemy";
}

CEnemy::~CEnemy()
{
}

void CEnemy::Init()
{
	__super::Init();

	getTransform().SetPosition(2.f, 2.f, 0.f);
	getTransform().SetLocalScale(1.5f, 1.5f, 1.f);
	m_pRenderer = m_pGameObject->AddComponent<CSpriteRenderer>();
	auto tex = CResources::GetInstance().getResource<CTexture>(L"Enemy").get();
	if (tex)
		m_pRenderer->SetTexture(tex);
	//m_pRenderer->SetTintColor(ColorValue::red());

	m_bInit = true;
}

void CEnemy::Awake()
{
	__super::Awake();
}

void CEnemy::Start()
{
	__super::Start();
}

void CEnemy::Update()
{
	__super::Update();

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

CEnemy* CEnemy::Create()
{
	return new CEnemy();
}
