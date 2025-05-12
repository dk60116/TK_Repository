#include "AudioSource.h"
#include "FMODSystem.h"

CAudioSource::CAudioSource()
	: m_pSystem(nullptr)
	, m_pChannel(nullptr)
	, m_pAudioClip(nullptr)
	, m_bLoop(false)
	, m_bPlayOnAwake(true)
	, m_fVolume(1.f)
{
	m_strName = L"Audio Source";
}

CAudioSource::~CAudioSource()
{
	OnDestroy();
}

void CAudioSource::Init()
{
	__super::Init();

	m_pSystem = CFMODSystem::GetInstance().getSystem();

	if (m_bPlayOnAwake)
		Play();
}

void CAudioSource::Awake()
{
	__super::Awake();
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

	if (m_pChannel)
	{
		_bool isPlaying = false;
		m_pChannel->isPlaying(&isPlaying);
		if (isPlaying)
			m_pChannel->stop();
		m_pChannel = nullptr;
	}

	m_pSystem = nullptr;
}

CAudioSource* CAudioSource::Create()
{
	return new CAudioSource();
}

HRESULT CAudioSource::SetClip(CAudioClip* _clip)
{
	if (!_clip)
		return E_FAIL;

	if (m_pAudioClip)
		Safe_Release(m_pAudioClip);

	m_pAudioClip = _clip;
	m_pAudioClip->AddRef();

	return S_OK;
}

_bool CAudioSource::Load()
{
	return _bool();
}

void CAudioSource::Play()
{
	if (!m_pSystem || !m_pAudioClip || !m_pAudioClip->getSound())
		return;

	FMOD_RESULT result = m_pSystem->playSound
	(
		m_pAudioClip->getSound(),
		nullptr,
		true,
		&m_pChannel
	);

	if (result != FMOD_OK || !m_pChannel)
		return;

	m_pChannel->setMode(m_bLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
	m_pChannel->setVolume(m_fVolume);
	m_pChannel->setPaused(false);
}

void CAudioSource::Stop()
{
	if (m_pChannel)
		m_pChannel->stop();
	m_pChannel = nullptr;
}

void CAudioSource::Pause()
{
	if (m_pChannel)
		m_pChannel->setPaused(true);
}

void CAudioSource::Resume()
{
	if (m_pChannel)
		m_pChannel->setPaused(false);
}

void CAudioSource::SetLoop(const _bool _loop)
{
	m_bLoop = _loop;

	if (m_pChannel)
		m_pChannel->setMode(_loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
}

void CAudioSource::SetVolume(const _float _volume)
{
	m_fVolume = _volume;

	if (m_pChannel)
		m_pChannel->setVolume(_volume);
}

void CAudioSource::SetPitch(const _float _pitch)
{
	if (m_pChannel)
		m_pChannel->setPitch(_pitch);
}

void CAudioSource::Set3DAttributes(const FMOD_VECTOR& _pos, const FMOD_VECTOR& _vel)
{
	if (m_pChannel)
		m_pChannel->set3DAttributes(&_pos, &_vel);
}
