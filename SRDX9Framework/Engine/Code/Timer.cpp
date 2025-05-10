#include "Timer.h"

CTimer::CTimer()
	: m_fTimeDelta(0.f)
	, m_fFPSTimer(0.f)
	, m_iFrameCount(0)
	, m_iFPS(0)
	, m_bFirstUpdate(true)
{
	ZeroMemory(&m_FixTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_LastTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_FrameTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
}

CTimer::~CTimer()
{
}

HRESULT CTimer::Ready_Timer()
{
	QueryPerformanceCounter(&m_FrameTime);			// 1077
	QueryPerformanceCounter(&m_LastTime);			// 1085
	QueryPerformanceCounter(&m_FixTime);			// 1090

	QueryPerformanceFrequency(&m_CpuTick);

    return S_OK;
}

void CTimer::Update()
{
	QueryPerformanceCounter(&m_FrameTime);

	if (m_FrameTime.QuadPart - m_FixTime.QuadPart >= m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_FixTime = m_FrameTime;
	}

	m_fTimeDelta = (m_FrameTime.QuadPart - m_LastTime.QuadPart) / (_float)m_CpuTick.QuadPart;

	if (m_bFirstUpdate)
	{
		m_fTimeDelta = 0.f;
		m_bFirstUpdate = false;
	}
	else
	{
		const float MAX_DELTA = 0.1f;
		m_fTimeDelta = min(m_fTimeDelta, MAX_DELTA);
	}

	m_LastTime = m_FrameTime;

	// FPS °è»ê
	m_fFPSTimer += m_fTimeDelta;
	++m_iFrameCount;

	if (m_fFPSTimer >= 1.f)
	{
		m_iFPS = m_iFrameCount;
		m_iFrameCount = 0;
		m_fFPSTimer = 0.f;
	}
}

void CTimer::Free()
{
}


