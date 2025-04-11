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

	if (result > 10.f)
		return 0.f;

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

void CTimeMgr::Update()
{
	m_pTimer->Update();
}

void CTimeMgr::Release()
{
	Safe_Delete(m_pTimer);
}
