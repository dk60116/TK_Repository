#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CAnimator final : public CComponent
{
    friend class CGameObject;

protected:
 	explicit CAnimator();
    ~CAnimator();

private:
    static CAnimator* Create();

public:
    HRESULT Initialize() override;
    void Update() override;
    void OnDestroy() override;

public:
    void Add_Animation(const wstring& _animName, CAnimation* _anim);
    void Set_PlaybackSpeed(const _float _value);

    void Play(const wstring& _animName);
    void SetLoop(const _bool _loop);

public:
    CAnimation* Get_CurrentAnimation();

private:
    class CSkinnedMeshRenderer* m_pSkinnedMesh;
    unordered_map<wstring, CAnimation*> m_mAnimationList;
    CAnimation* m_pCrtAnimation;
    _bool m_bIsPlaying, m_bLoop;
    _float m_fCurrentTime;
    _float m_fPlaybackSpeed;
};

NS_END

