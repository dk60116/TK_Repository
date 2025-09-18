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

	if (!_desc)
		return E_FAIL;

	PARTICLEDESC particleInfo = *static_cast<PARTICLEDESC*>(_desc);

	m_vParticleList = particleInfo.particles;

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

CMeshBuffer* CParticleSystem::CreateInstaceBuffer()
{
	return nullptr;
}
