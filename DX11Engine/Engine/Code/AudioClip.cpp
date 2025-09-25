#include "epch.h"
#include "AudioClip.h"

CAudioClip::CAudioClip()
	: m_pSound(nullptr)
{
}

CAudioClip::~CAudioClip()
{
}

CAudioClip* CAudioClip::Create()
{
	return new CAudioClip();
}

HRESULT CAudioClip::Initialize(const wstring& _name, const wstring& _filePath, void* _desc)
{
	if (FAILED(__super::Initialize(_name, _filePath, _desc)))
		return E_FAIL;

	if (m_strFilePath.empty())
		return E_FAIL;

	string pathA = CEngineString::WStringToString(m_strFilePath);

	if (!CFMODSystem::GetInstance().GetSystem())
		return E_FAIL;

	FMOD_RESULT result = CFMODSystem::GetInstance().GetSystem()->createSound(pathA.c_str(), FMOD_DEFAULT, nullptr, &m_pSound);

	if (result != FMOD_OK)
		return E_FAIL;

	return S_OK;
}

void CAudioClip::OnDestroy()
{
	m_pSound->release();
}

FMOD::Sound* CAudioClip::getSound() const
{
	return m_pSound;
}
