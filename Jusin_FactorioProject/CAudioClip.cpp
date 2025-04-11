#include "CAudioClip.h"

CAudioClip::CAudioClip()
	: m_pSound(nullptr)
{
}

CAudioClip::~CAudioClip()
{
	Release();
}

void CAudioClip::Release()
{
	if (m_pSound)
	{
		m_pSound->release();
		m_pSound = nullptr;
	}
}

void CAudioClip::LoadSound(const char* _filePath)
{
    FMOD::System* system = CAudioManager::GetInstance().getSystem();

    if (!system)
        return;

    FMOD_RESULT result = system->createSound(_filePath, FMOD_DEFAULT, 0, &m_pSound);
    if (result != FMOD_OK)
        m_pSound = nullptr;
}
