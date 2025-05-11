#include "AudioSource.h"
#include "FMODSystem.h"

CAudioSource::CAudioSource()
	: m_pSystem(nullptr)
	, m_bLoop(false)
	, m_bPlayOnAwake(true)
	, m_fVolume(1.f)
{
}

CAudioSource::~CAudioSource()
{
	OnDestroy();
}

void CAudioSource::Awake()
{
	__super::Awake();

	m_pSystem = CFMODSystem::GetInstance().getSystem();
}

void CAudioSource::Start()
{
}

void CAudioSource::UpdateEditor()
{
}

void CAudioSource::Update()
{
}

void CAudioSource::FixedUpdate()
{
}

void CAudioSource::LateUpdate()
{
}

void CAudioSource::Render()
{
}

void CAudioSource::OnEnable()
{
}

void CAudioSource::OnDisable()
{
}

void CAudioSource::OnDestroy()
{
	__super::OnDestroy();
}

CAudioSource* CAudioSource::Create()
{
	return new CAudioSource();
}

_bool CAudioSource::Load()
{
	return _bool();
}

void CAudioSource::Play()
{
}

void CAudioSource::Stop()
{
}

void CAudioSource::Pause()
{
}

void CAudioSource::Resume()
{
}

void CAudioSource::SetLoop(_bool _loop)
{
}

void CAudioSource::SetVolume(_float _volume)
{
}

void CAudioSource::SetPitch(_float _pitch)
{
}

void CAudioSource::Set3DAttributes(const FMOD_VECTOR& pos, const FMOD_VECTOR& vel)
{
}
