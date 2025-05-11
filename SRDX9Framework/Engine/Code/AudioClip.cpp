#include "AudioClip.h"
#include "FMODSystem.h"

CAudioClip::CAudioClip()
	: m_pSound(nullptr)
{
}

CAudioClip::~CAudioClip()
{
	Destroy();
}

HRESULT CAudioClip::Load(LPDIRECT3DDEVICE9 _device)
{
	UNREFERENCED_PARAMETER(_device);

	Destroy();

	if (m_strFilePath.empty())
		return E_FAIL;

	string pathA(m_strFilePath.begin(), m_strFilePath.end());

	if (!CFMODSystem::GetInstance().getSystem())
		return E_FAIL;

	FMOD_RESULT result = CFMODSystem::GetInstance().getSystem()->createSound(pathA.c_str(), FMOD_DEFAULT, nullptr, &m_pSound);
	
	if (result != FMOD_OK)
		return E_FAIL;

	CDebug::Log(L"[AudioClip::Load] File = " + m_strFilePath);

	return S_OK;
}

void CAudioClip::Destroy()
{
	__super::Destroy();
}
