#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CAudioSource final : public CComponent
{
private:
	CAudioSource();
	~CAudioSource();

public:
	HRESULT Initialize(void* _desc) override;
	void Awake() override;
	void Update() override;
	void OnEnable() override;
	void OnDisable() override;
	void OnDestroy() override;

public:
	static CAudioSource* Create();
	CComponent* Clone() const override;

public:
	void SetClip(CAudioClip* _clip);
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
	_float m_fPitch;

	BEGIN_SERIALIZEFIELD
	SERIALIZEFIELD(m_pAudioClip)
	SERIALIZEFIELD(m_bPlayOnAwake)
	SERIALIZEFIELD(m_bLoop)
	SERIALIZEFIELD(m_fVolume)
	END_SERIALIZEFIELD
};

NS_END

