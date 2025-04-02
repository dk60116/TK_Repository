#pragma once

#include "CMonoBehaviour.h"
#include "CAudioManager.h"
#include "CAudioClip.h"

struct AudioOptions
{
    bool playOnAwake = false;
    bool loop = false;
    float volume = 1.f;
    float pitch = 1.f;
};

class CAudioSource :
    public CMonoBehaviour
{
public:
    CAudioSource();
    ~CAudioSource();

public:
    void Awake() override;
    void Start() override;
    void Reset() override;
    void Update() override;
    void FixedUpdate() override;
    void LateUpdate() override;
    void OnEnable() override;
    void OnDisable() override;
    void Render(HDC _hDC) override;
    void OnCollisionEnter(CCollider* _other) override;
    void OnCollisionStay(CCollider* _other) override;
    void OnCollisionExit(CCollider* _other) override;
    void OnDestroy() override;
    void OnMouseEnter() override;
    void OnMouseOver() override;
    void OnMouseExit() override;

public:
    void SetClip(wstring _clip);
    void Play();
    void Stop();
    void Pause(bool pause);
    bool IsPlaying();

    AudioOptions& getOptions() { return m_sOptions; }

private:
    CAudioClip* m_pAudioResource;
    FMOD::Channel* m_pChannel;

    AudioOptions m_sOptions;
};

