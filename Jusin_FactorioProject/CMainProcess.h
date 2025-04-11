#pragma once

#include "pch.h"

class CMainProcess
{
	SingletonClass(CMainProcess);

public:
	void		Initialize();
	void		MainLoop();
	void		MainDisplay();
	void		Release();

private:
	HDC		m_hDC;

	ULONG		m_dwTime;
	int			m_iFPS;
	TCHAR		m_szFPS[16];

	DWORD m_fFixedCountTime;
};

