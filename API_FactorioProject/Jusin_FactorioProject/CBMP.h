#pragma once

#include "pch.h"

class CBMP
{
public:
	CBMP();
	~CBMP();

public:
	HDC Get_MemDC() { return m_hMemDC; }

public:
	void Load_Bmp(const TCHAR* _filePath);
	void Release();

private:
	HDC m_hMemDC;

	HBITMAP m_hBitmap;
	HBITMAP m_hOldbmp;
};

