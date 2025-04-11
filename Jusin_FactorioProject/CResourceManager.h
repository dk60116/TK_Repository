#pragma once

#include "pch.h"
#include "CBMP.h"
#include "CAudioClip.h"

class CResourceManager
{
	SingletonClass(CResourceManager);

public:
	void Init();
	void Release();

public:
	void Insert_Bmp(const TCHAR* _filePath, const wstring _imgKey);
	void Insert_AudioClip(const char* _filePath, const wstring _audioKey);
	CBMP* FindBMP(const wstring _name);
	HDC	Find_Sprite(const wstring _imgKey);
	CAudioClip* Find_AudioClip(const wstring _audioKey);

private:
	map<const wstring, CBMP*> m_mBmpList;
	map<const wstring, CAudioClip*> m_mAuidoList;
};

