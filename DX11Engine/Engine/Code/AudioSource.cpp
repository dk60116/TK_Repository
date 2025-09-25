#include "epch.h"

CAudioSource::CAudioSource()
	: m_pSystem(nullptr)
	, m_pChannel(nullptr)
	, m_pAudioClip(nullptr)
	, m_bPlayOnAwake(nullptr)
	, m_bLoop(false)
	, m_fVolume(1.f)
	, m_fPitch(1.f)
{
	m_strName = L"Audio Source";
}

CAudioSource::~CAudioSource()
{
}

HRESULT CAudioSource::Initialize(void* _desc)
{
	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	m_pSystem = CFMODSystem::GetInstance().GetSystem();

	return S_OK;
}

void CAudioSource::Awake()
{
}

void CAudioSource::Update()
{
}

void CAudioSource::OnEnable()
{
	if (m_bPlayOnAwake)
		Play();
}

void CAudioSource::OnDisable()
{
}

void CAudioSource::OnDestroy()
{
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

CComponent* CAudioSource::Clone() const
{
	CAudioSource* clone = new CAudioSource();

	clone->SetClip(this->m_pAudioClip);

	return clone;
}

void CAudioSource::SetClip(CAudioClip* _clip)
{
	if (_clip == m_pAudioClip)
		return;
	
	Safe_Release(m_pAudioClip);

	m_pAudioClip = _clip;

	if (m_pAudioClip)
		m_pAudioClip->AddRef();
}

void CAudioSource::Play()
{
	if (!m_pSystem)
		return;

	if (!m_pAudioClip)
	{
		CDebug::LogError(L"Audio source can not play - AudioClip is nullprt: " + m_pGameObject->Get_ObjectNameID());
		return;
	}

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

void CAudioSource::SetLoop(const _bool _loop)
{
	m_bLoop = _loop;
}

void CAudioSource::SetVolume(const _float _volume)
{
	m_fVolume = clamp(_volume, 0.f, 1.f);

	if (m_pChannel)
		m_pChannel->setVolume(m_fVolume);
}

void CAudioSource::SetPitch(const _float _pitch)
{
	m_fPitch = clamp(_pitch, 0.f, 10.f);

	if (m_pChannel)
		m_pChannel->setPitch(m_fPitch);
}

void CAudioSource::Set3DAttributes(const FMOD_VECTOR& _pos, const FMOD_VECTOR& _vel)
{
}

void CAudioSource::Resume()
{
	if (m_pChannel)
		m_pChannel->setPaused(false);
}
