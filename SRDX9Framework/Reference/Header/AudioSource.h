#pragma once

#include "Component.h"

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
	_bool Load();
	void Play();
	void Stop();
	void Pause();
	void Resume();

	void SetLoop(_bool _loop);
	void SetVolume(_float _volume);
	void SetPitch(_float _pitch);
	void Set3DAttributes(const FMOD_VECTOR& pos, const FMOD_VECTOR& vel);

private:
	FMOD::System* m_pSystem;
	FMOD::Sound* m_pSound;
	FMOD::Channel* m_pChannel;

	_bool m_bPlayOnAwake;
	_bool m_bLoop;
	_float m_fVolume;

	BEGIN_SERIALIZEFIELD
	END_SERIALIZEFIELD
};

END
