#pragma once

#include "CMonoBehaviour.h"
#include "CAnimation.h"
#include "CSpriteRenderer.h"

class CBeltManager;

class CAnimator :
    public CMonoBehaviour
{
public:
    CAnimator();
    ~CAnimator();

    void Awake() override;
    void Start() override;
    void Reset() override;
    void Update() override;
    void FixedUpdate() override;
    void LateUpdate() override;
    void OnEnable() override;
    void OnDisable() override;
    void Render(HDC _hdc) override;
    void OnCollisionEnter(CCollider* _other) override;
    void OnCollisionStay(CCollider* _other) override;
    void OnCollisionExit(CCollider* _other) override;
    void OnDestroy() override;
    void OnMouseEnter() override;
    void OnMouseOver() override;
    void OnMouseExit() override;

public:
    void GetAnimationDisplay(HDC& _hDC, CTransform& _tf, vector2 _position, float _size, const ColorValue _tpColor);

public:
    void Play();
    void Pause();
    void Stop();

public:
    void SetRenderer(CSpriteRenderer* _renderer) { m_pRenderer = _renderer; }
    void CreateAnimation(wstring _name, wstring _sprite, int _count, vector2 _offset, vector2Int _xyLength, int _yValue, float _frameInterval = 0.1f, bool _loop = true);
    void SetAnimation(wstring _lastName);
    void SetAnimation(int _ySlice);
    const int getCurrentAnimationFrame();

private:
    CSpriteRenderer* m_pRenderer;
    map<wstring, CAnimation*> m_mAnimationList;
    CAnimation* m_pCurrentAnimation;
    float m_fCurrnetTime;

    bool m_bIsPlaying;

    friend class CAnimation;
    friend class CSpriteRenderer;
    friend class CBeltManager;
};

