#include "CTime.h"
#include "CScene.h"

CTime::CTime()
    : m_llFrequency({ 0 })
    , m_llOldCount({ 0 })
    , m_dDeltaTime(0)
{
    QueryPerformanceFrequency(&m_llFrequency);
    QueryPerformanceCounter(&m_llOldCount);
}

CTime::~CTime()
{
}

void CTime::Update()
{
    LARGE_INTEGER llCurCount;
    QueryPerformanceCounter(&llCurCount);

    LONGLONG diff = llCurCount.QuadPart - m_llOldCount.QuadPart;

    m_dDeltaTime = (double)diff / (double)m_llFrequency.QuadPart;

    m_llOldCount = llCurCount;
}

void CTime::Render(HDC hDC)
{
}

void CTime::Release()
{
}
