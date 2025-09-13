#include "cpch.h"
#include "ME_FireBall.h"

CME_FireBall::CME_FireBall()
	: CMeshEffect{}
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

	m_sDescription.meshCount = 1;

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_vMesheList[0]->Get_Material()->Set_BaseColor(ColorValue::red().f4Color());

	return S_OK;
}

void CME_FireBall::Awake()
{
	__super::Awake();
}

void CME_FireBall::Start()
{
	__super::Start();
}

void CME_FireBall::Update()
{
	__super::Update();
}

void CME_FireBall::OnDestroy()
{
	__super::OnDestroy();
}
