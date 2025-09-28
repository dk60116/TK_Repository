#include "cpch.h"
#include "ME_FireBall.h"

CME_FireBall::CME_FireBall()
	: CMeshEffect{}
	, m_pLight(nullptr)
	, m_fRotYSpeed(0.f)
	, m_bShooting(false)
	, m_vTargetPos({})
	, m_vDirection({})
	, m_pCollider(nullptr)
	, m_pParticle(nullptr)
{
}

CME_FireBall::~CME_FireBall()
{
}

CME_FireBall* CME_FireBall::Create()
{
	return new CME_FireBall();
}

CComponent* CME_FireBall::Clone() const
{
	CME_FireBall* clone = new CME_FireBall();

	return clone;
}

HRESULT CME_FireBall::Initialize(void* _desc)
{
	m_strEffectName = L"FireBall";

	m_sDescription.meshCount = 3;

	m_fRotYSpeed = 180.f;

	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	if (!m_pLight)
	{
		m_pLight = m_pGameObject->AddComponent<CLight>();
		m_pLight->Set_Type(CLight::Type::Point);
		m_pLight->Set_Range(5.f);
		m_pLight->Set_Color(ColorValue::red());
	}

	ColorValue red = ColorValue::red();
	red.a = 150;

	ColorValue black = ColorValue::black();
	black.a = 200;
	
	m_vMesheList[0]->Get_Material()->Set_BaseColor(red.f4Color());
	m_vMesheList[0]->Get_Material()->Set_Texture(CResources::LoadOnScene<CTexture>(L"NoiseTexture (Texture)"), 3);

	m_vMesheList[1]->Get_Transform()->Set_LocalPositionY(-0.3f);
	m_vMesheList[1]->Get_Transform()->Set_LocalScale(0.95f, 1.2f, 0.95f);

	m_vMesheList[1]->Get_Material()->Set_BaseColor(black.f4Color());
	m_vMesheList[1]->Get_Material()->Set_Texture(CResources::LoadOnScene<CTexture>(L"NoiseTexture (Texture)"), 3);

	m_vMesheList[2]->Get_Transform()->Set_LocalPositionY(-1.2f);
	m_vMesheList[2]->Get_Transform()->Set_LocalScale(0.005f, 0.01f, 0.005f);
	m_vMesheList[2]->Get_Material()->Set_BaseColor(red.f4Color());

	m_pCollider = m_pGameObject->AddComponent<CSphereCollider>();
	m_pCollider->Set_Size(1.5f);
	m_pCollider->SetTrigger(true);

	CGameObject* ptcObj = m_pGameObject->Get_Scene()->Add_GameObject(L"Fire Particle");
	CParticleSystem::PARTICLEDESC desc = {};
	desc.particles[0].info.loop = false;
	desc.particles[0].info.startDelay = 0.f;
	desc.particles[0].info.startLifeTime = 1.f;
	desc.particles[0].info.startColor = ColorValue::black();

	m_pParticle = ptcObj->AddComponent<CParticleSystem>(&desc);

	return S_OK;
}

void CME_FireBall::Awake()
{
	__super::Awake();

	Get_Transform()->Add_PositionY(2.f);
}

void CME_FireBall::Start()
{
	__super::Start();
}

void CME_FireBall::Update()
{
	__super::Update();

	m_fTime += DELTA_TIME;

	m_vMesheList[0]->Get_Material()->Set_FloatValue(L"gTime", m_fTime);
	m_vMesheList[0]->Get_Transform()->Add_LocalEulerAnglesY(m_fRotYSpeed * DELTA_TIME);

	m_vMesheList[1]->Get_Material()->Set_FloatValue(L"gTime", m_fTime);
	m_vMesheList[1]->Get_Transform()->Add_LocalEulerAnglesY(m_fRotYSpeed * DELTA_TIME);

	m_vMesheList[1]->Get_Material()->Set_FloatValue(L"gTime", m_fTime);
	m_vMesheList[2]->Get_Transform()->Add_LocalEulerAnglesY(-m_fRotYSpeed * 2.f * DELTA_TIME);

	if (m_bShooting)
		Shooting();
}

void CME_FireBall::OnTriggerEnter(CCollider* _other)
{
	if (_other->Get_GameObject()->CompareTag(L"Player"))
	{
		CGameManager::GetInstance().Get_Player()->GetDamage(1);
	}

	m_pParticle->Get_Transform()->Set_Position(Get_Transform()->Get_Position());
	m_pParticle->Get_GameObject()->SetActive(true);
}

void CME_FireBall::OnDestroy()
{
	__super::OnDestroy();
}

void CME_FireBall::Shoot(const vector3& _startPos, const vector3& _targetPos)
{
	m_pParticle->Get_GameObject()->SetActive(false);

	Get_Transform()->Set_Position(_startPos);
	Get_Transform()->LookAt(_targetPos);
	Get_Transform()->Add_LocalEulerAnglesX(90.f);

	m_vDirection = (_targetPos -_startPos).normalized();
	m_bShooting = true;
}

void CME_FireBall::Shooting()
{
	CTransform* myTF = Get_Transform();

	myTF->Add_Position(m_vDirection * DELTA_TIME * 20.f);
}
