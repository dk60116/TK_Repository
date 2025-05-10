#include "ETime.h"

CTime::CTime()
	:m_pTimer(nullptr)
	, m_fTimeScale(1.f)
{
}

CTime::~CTime()
{
	Release();
}

_int CTime::Get_FPS()
{
	_int result = m_pTimer->Get_FPS();

	return result;
}

_float CTime::Get_DeltaTime()
{
	float result = m_pTimer->Get_TimeDelta() * m_fTimeScale;

	if (result > 10.f)
		return 0.f;

	return result;
}

HRESULT CTime::Ready_Timer()
{
	m_pTimer = new CTimer();

	if (!m_pTimer)
	{
		return E_FAIL;
	}

	return S_OK;
}

void CTime::Update()
{
	m_pTimer->Update();
}

void CTime::Release()
{
	Safe_Delete(m_pTimer);
}
