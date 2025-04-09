#include "CTimeMgr.h"

CTimeMgr::CTimeMgr()
	:m_pTimer(nullptr)
	, m_fTimeScale(1.f)
{
}

CTimeMgr::~CTimeMgr()
{
	Release();
}

_int CTimeMgr::Get_FPS()
{
	_int result = m_pTimer->Get_FPS();

	return result;
}

_float CTimeMgr::Get_TimeDelta()
{
	float result = m_pTimer->Get_TimeDelta() * m_fTimeScale;

	return result;
}

HRESULT CTimeMgr::Ready_Timer()
{
	m_pTimer = new CTimer();

	if (!m_pTimer)
	{
		return E_FAIL;
	}

	return S_OK;
}

void CTimeMgr::Update_Timer()
{
	m_pTimer->Update_Timer();
}

void CTimeMgr::Release()
{
	Safe_Delete(m_pTimer);
}
