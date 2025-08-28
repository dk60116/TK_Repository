#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CAnimator final : public CComponent
{
    friend class CGameObject;

public:
    struct AnimatorStateInfo
    {
        _float length;
        _float normalizeTime;
    };

protected:
 	explicit CAnimator();
    ~CAnimator();

private:
    static CAnimator* Create();
    CComponent* Clone() const override;

public:
    HRESULT Initialize() override;
    void Update() override;
    void OnDestroy() override;

public:
    void Add_Animation(const wstring& _animName, CAnimationClip* _anim);
    void Set_PlaybackSpeed(const _float _value);

    void Play();
    void Play(const wstring& _animName, const _float _blendDuration = 0.f);
    void Pause();
    void Stop();

    void SetLoop(const _bool _loop);
    void SetSpeed(const _float _value);

public:
    CAnimationClip* Get_CurrentAnimation();
    const wstring& Get_CurrentAnimationName();
    AnimatorStateInfo& Get_StateInfo();

private:
    class CSkinnedMeshRenderer* m_pSkinnedRenderer;
    unordered_map<wstring, CAnimationClip*> m_mAnimationList;
    CAnimationClip* m_pCrtAnimation, * m_pNextAnimation;
    wstring m_strCrtAnimName;
    _bool m_bIsPlaying, m_bBlending, m_bLoop;
    _float m_fCurrentTime, m_fBlendTime, m_fBlendDuration;
    _float m_fPlaybackSpeed;
    vector<_matrix> m_vFinalBoneMatrix;
	AnimatorStateInfo m_sStateInfo;

    unordered_map<wstring, CAnimationClip::BoneTransform> m_mBlendStartPose;

    BEGIN_SERIALIZEFIELD
        SERIALIZEFIELD(m_pSkinnedRenderer)
        SERIALIZEFIELD(m_pCrtAnimation)
    END_SERIALIZEFIELD
};

NS_END

