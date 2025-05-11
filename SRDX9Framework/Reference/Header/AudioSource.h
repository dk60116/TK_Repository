#pragma once

#include "Component.h"
#include "AudioClip.h"

BEGIN(Engine)

class ENGINE_DLL CAudioSource 
	: public CComponent
{
private:
    CAudioSource();
    ~CAudioSource();

public:
	void Awake() override;
	void Start() override;
	void UpdateEditor() override;
	void Update() override;
	void FixedUpdate() override;
	void LateUpdate() override;
	void Render() override;
	void OnEnable() override;
	void OnDisable() override;
	void OnDestroy() override;

public:
	static CAudioSource* Create();

public:
	HRESULT SetClip(CAudioClip* _clip);
	_bool Load();
	void Play();
	void Stop();
	void Pause();
	void Resume();

	void SetLoop(const _bool _loop);
	void SetVolume(const _float _volume);
	void SetPitch(const _float _pitch);
	void Set3DAttributes(const FMOD_VECTOR& _pos, const FMOD_VECTOR& _vel);

private:
	FMOD::System* m_pSystem;
	FMOD::Channel* m_pChannel;

	CAudioClip* m_pAudioClip;

	_bool m_bPlayOnAwake;
	_bool m_bLoop;
	_float m_fVolume;

	BEGIN_SERIALIZEFIELD
	SERIALIZEFIELD(m_pAudioClip)
	SERIALIZEFIELD(m_bPlayOnAwake)
	SERIALIZEFIELD(m_bLoop)
	SERIALIZEFIELD(m_fVolume)
	END_SERIALIZEFIELD
};

END
