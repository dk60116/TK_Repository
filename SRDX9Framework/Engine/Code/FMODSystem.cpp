#include "FMODSystem.h"

CFMODSystem::CFMODSystem()
{
}

CFMODSystem::~CFMODSystem()
{
	Release();
}

HRESULT CFMODSystem::Init()
{
	FMOD_RESULT result;

	result = FMOD::System_Create(&m_pSystem);

	if (result != FMOD_OK)
		return E_FAIL;

	result = m_pSystem->init(512, FMOD_INIT_NORMAL, nullptr);

	if (result != FMOD_OK)
		return E_FAIL;

	return S_OK;
}

void CFMODSystem::Update()
{
	if (m_pSystem)
		m_pSystem->update();
}

void CFMODSystem::Release()
{
	if (!m_pSystem)
		return;

	m_pSystem->close();
	m_pSystem->release();
	m_pSystem = nullptr;

	Release();
}
