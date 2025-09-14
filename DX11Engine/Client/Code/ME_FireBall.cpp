#include "cpch.h"
#include "ME_FireBall.h"

CME_FireBall::CME_FireBall()
	: CMeshEffect{}
	, m_fRotYSpeed(0.f)
	, m_fTime(0.f)
	, m_bShooting(false)
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

HRESULT CME_FireBall::Initialize()
{
	m_strEffectName = L"FireBall";

	m_sDescription.meshCount = 3;

	m_fRotYSpeed = 180.f;

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	ColorValue red = ColorValue::red();
	red.a = 150;

	ColorValue black = ColorValue::black();
	black.a = 200;
	
	m_vMesheList[0]->Get_Material()->Set_BaseColor(red.f4Color());
	m_vMesheList[0]->Get_Material()->Set_Texture(CResources::LoadOnScene<CTexture>(L"NoiseTexture (Texture)"), 1);

	m_vMesheList[1]->Get_Transform()->Set_LocalPositionY(-0.3f);
	m_vMesheList[1]->Get_Transform()->Set_LocalScale(0.95f, 1.2f, 0.95f);

	m_vMesheList[1]->Get_Material()->Set_BaseColor(black.f4Color());
	m_vMesheList[1]->Get_Material()->Set_Texture(CResources::LoadOnScene<CTexture>(L"NoiseTexture (Texture)"), 1);

	m_vMesheList[2]->Get_Transform()->Set_LocalPositionY(-1.2f);
	m_vMesheList[2]->Get_Transform()->Set_LocalScale(0.005f, 0.01f, 0.005f);
	m_vMesheList[2]->Get_Material()->Set_BaseColor(red.f4Color());

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

	//m_vMesheList[1]->Get_Material()->Set_FloatValue(L"gTime", m_fTime);
	m_vMesheList[2]->Get_Transform()->Add_LocalEulerAnglesY(m_fRotYSpeed * 2.f * DELTA_TIME);

	if (m_bShooting)
		Shooting();
}

void CME_FireBall::OnDestroy()
{
	__super::OnDestroy();
}

void CME_FireBall::Shoot(const vector3& _startPos, const vector3& _targetPos)
{
	Get_Transform()->Set_Position(_startPos);
	Get_Transform()->LookAt(_targetPos);
	Get_Transform()->Add_LocalEulerAnglesX(90.f);
	m_bShooting = true;
}

void CME_FireBall::Shooting()
{
	CTransform* myTF = Get_Transform();

	myTF->Add_Position(myTF->Get_Directions().up * DELTA_TIME * 20.f);
}
