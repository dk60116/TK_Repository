#include "epch.h"
#include "FMODSystem.h"

CFMODSystem::CFMODSystem()
	: m_pSystem(nullptr)
{
}

CFMODSystem::~CFMODSystem()
{
}

CFMODSystem& CFMODSystem::GetInstance()
{
	static CFMODSystem inst;
	return inst;
}

HRESULT CFMODSystem::Initialize()
{
	FMOD_RESULT result = FMOD_OK;

	result = FMOD::System_Create(&GetInstance().m_pSystem);

	if (result != FMOD_OK)
		return E_FAIL;

	result = GetInstance().m_pSystem->init(512, FMOD_INIT_NORMAL, nullptr);

	if (result != FMOD_OK)
		return E_FAIL;

	return S_OK;
}

void CFMODSystem::Update()
{
	if (GetInstance().m_pSystem)
		GetInstance().m_pSystem->update();
}

void CFMODSystem::Release()
{
	if (GetInstance().m_pSystem)
	{
		GetInstance().m_pSystem->close();
		GetInstance().m_pSystem->release();
		GetInstance().m_pSystem = nullptr;
	}
}

FMOD::System* CFMODSystem::GetSystem() const
{
	return m_pSystem;
}
