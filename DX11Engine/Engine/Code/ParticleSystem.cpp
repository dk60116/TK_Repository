#include "epch.h"
#include "ParticleSystem.h"

CParticleSystem::CParticleSystem()
	: m_vParticleList({})
{
}

CParticleSystem::~CParticleSystem()
{
}

CParticleSystem* CParticleSystem::Create()
{
	return new CParticleSystem();
}

CComponent* CParticleSystem::Clone() const
{
	CParticleSystem* clone = new CParticleSystem();

	return clone;
}

HRESULT CParticleSystem::Initialize(void* _desc)
{
	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	return S_OK;
}

void CParticleSystem::Update()
{
}

void CParticleSystem::Render_Editor()
{
}

void CParticleSystem::Render()
{
}

void CParticleSystem::OnDestroy()
{
}
