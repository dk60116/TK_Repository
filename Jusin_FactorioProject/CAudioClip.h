#pragma once

#include "pch.h"
#include "CAudioManager.h"

class CAudioClip
{
public:
	CAudioClip();
	~CAudioClip();

public:
	void Release();

public:
	void LoadSound(const char* _filePath);
	FMOD::Sound* getSound() const { return m_pSound; }

private:
	FMOD::Sound* m_pSound;
};

