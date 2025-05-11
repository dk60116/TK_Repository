#include "FMODSystem.h"

CFMODSystem::CFMODSystem()
	: m_pSystem(nullptr)
{
}

CFMODSystem::~CFMODSystem()
{
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

void CFMODSystem::Free()
{
	if (m_pSystem)
	{
		m_pSystem->close();
		m_pSystem->release();
		m_pSystem = nullptr;
	}
}
