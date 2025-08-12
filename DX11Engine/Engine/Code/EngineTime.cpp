#include "epch.h"
#include "EngineTime.h"

CTime::CTime()
	: m_pTimer(nullptr)
	, m_fTimeScale(0.f)
{
}

CTime::~CTime()
{
	Release();
}

CTime& CTime::GetInstance()
{
	static CTime inst;
	return inst;
}

HRESULT CTime::Initialize()
{
	if (FAILED(Ready_Time()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTime::Ready_Time()
{
	GetInstance().m_pTimer = new CTimer();

	if (!GetInstance().m_pTimer)
		return E_FAIL;

	GetInstance().m_pTimer->AddRef();

	if (FAILED(GetInstance().m_pTimer->Ready_Timer()))
	{
		delete GetInstance().m_pTimer;
		GetInstance().m_pTimer = nullptr;
		E_FAIL;
	}

	return S_OK;
}

void CTime::Update()
{
	if (!GetInstance().m_pTimer)
		return;

	GetInstance().m_pTimer->Update();
}

_int CTime::Get_FPS()
{
	return GetInstance().m_pTimer->Get_FPS();
}

_float CTime::Get_DeltaTime()
{
	return GetInstance().m_pTimer->Get_TimeDelta();
}

_float CTime::Get_ElaspedTime()
{
	return GetInstance().m_pTimer->Get_ElapsedTime();
}

void CTime::SetTimeScale(const _float _value)
{
	GetInstance().m_fTimeScale = _value;
}

void CTime::Release()
{
	Safe_Release(GetInstance().m_pTimer);
}
