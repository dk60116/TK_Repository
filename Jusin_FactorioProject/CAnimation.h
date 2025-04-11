#pragma once

#include "pch.h"
#include "CBMP.h"

class CAnimator;
class CBeltManager;

class CAnimation
{
public:
    CAnimation();
    ~CAnimation();

public:
    void Init(CAnimator* _animator, HDC _image, int _frameCount, vector2 m_v2Offset, vector2Int _xyLength, int _yValue, float _frameInterval, bool _loop);

public:
    void DisplayFrame(HDC& _hDC, CTransform& _tf, vector2 _position, float _size, const ColorValue _tpColor);

private:
    CAnimator* m_pAnimator;
    HDC m_hMemDC;
    int m_iFrameCount;
    int m_iCurrentFrame;
    vector2 m_v2Offset;
    vector2Int m_v2xyLength;
    int m_iYvalue;
    vector2 m_v2CurrentPos;
    bool m_bLoop;
    float m_fFrameInterval;
    float m_fSpeed;

    friend class CAnimator;
    friend class CBeltManager;
};

