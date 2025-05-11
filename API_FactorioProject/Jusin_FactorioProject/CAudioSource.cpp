#include "CAudioSource.h"
#include "CResourceManager.h"

CAudioSource::CAudioSource()
	: m_pAudioResource(nullptr)
	, m_pChannel(nullptr)
	, m_sOptions({})
{
}

CAudioSource::~CAudioSource()
{
    OnDestroy_();
}

void CAudioSource::Awake()
{
    Awake_();
}

void CAudioSource::Start()
{
    Start_();

    if (m_sOptions.playOnAwake)
        Play();
}

void CAudioSource::Reset()
{
    Reset_();
}

void CAudioSource::Update()
{
    Update_();

    if (m_pChannel)
        m_pChannel->setVolume(m_sOptions.volume);
}

void CAudioSource::FixedUpdate()
{
    FixedUpdate_();
}

void CAudioSource::LateUpdate()
{
    LateUpdate_();
}

void CAudioSource::OnEnable()
{
    OnEnable_();
}

void CAudioSource::OnDisable()
{
    OnDisable_();
}

void CAudioSource::Render(HDC _hDC)
{
}

void CAudioSource::OnCollisionEnter(CCollider* _other)
{
}

void CAudioSource::OnCollisionStay(CCollider* _other)
{
    
}

void CAudioSource::OnCollisionExit(CCollider* _other)
{
}

void CAudioSource::OnDestroy()
{
    OnDestroy_();

    Stop();
}

void CAudioSource::OnMouseEnter()
{
}

void CAudioSource::OnMouseOver()
{
}

void CAudioSource::OnMouseExit()
{
}

void CAudioSource::SetClip(wstring _clip)
{
    m_pAudioResource = CResourceManager::GetInstance().Find_AudioClip(_clip);
}

void CAudioSource::Play()
{
    if (!m_pAudioResource)
        return;

    FMOD::System* system = CAudioManager::GetInstance().getSystem();
    if (!system)
        return;

    if (m_pChannel)
    {
        bool isPlaying = false;
        m_pChannel->isPlaying(&isPlaying);
        if (isPlaying)
            m_pChannel->stop();
    }

    FMOD_RESULT result = system->playSound(m_pAudioResource->getSound(), nullptr, false, &m_pChannel);

    if (result != FMOD_OK)
        return;

    if (m_sOptions.loop)
        m_pChannel->setMode(FMOD_LOOP_NORMAL);
    else
        m_pChannel->setMode(FMOD_LOOP_OFF);
}

void CAudioSource::Stop()
{
    m_pChannel->stop();
}

void CAudioSource::Pause(bool pause)
{
    if (m_pChannel)
        m_pChannel->setPaused(pause);
}

bool CAudioSource::IsPlaying()
{
    if (!m_pChannel)
        return false;

    bool isPlaying = false;
    m_pChannel->isPlaying(&isPlaying);
    return isPlaying;
}
