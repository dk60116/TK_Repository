#include "epch.h"
#include "EngineTime.h"

CTime::CTime()
	: m_pTimer(nullptr)
	, m_fTimeScale(0.f)
{
}

CTime::~CTime()
{
	Destroy();
}

HRESULT CTime::Initialize()
{
	if (FAILED(Ready_Time()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTime::Ready_Time()
{
	m_pTimer = new CTimer();

	if (!m_pTimer)
		return E_FAIL;

	m_pTimer->AddRef();

	m_pTimer->Ready_Timer();

	return S_OK;
}

void CTime::Update()
{
	m_pTimer->Update();
}

_int CTime::Get_FPS()
{
	return m_pTimer->Get_FPS();
}

_float CTime::Get_DeltaTime()
{
	return m_pTimer->Get_TimeDelta();
}

void CTime::SetTimeScale(const float _value)
{
	m_fTimeScale = _value;
}

void CTime::Destroy()
{
	Safe_Release(m_pTimer);
}
