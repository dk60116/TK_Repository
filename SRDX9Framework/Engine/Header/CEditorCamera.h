#pragma once

#include "CCamera.h"

BEGIN(Engine)

class ENGINE_DLL CEditorCamera 
    : public CCamera
{
public:
    CEditorCamera();
    ~CEditorCamera();

public:
    void UpdateEditor() override;

private:
    float m_fMoveSpeed, m_fRotateSpeed;
    _bool m_bRMouseDowned;
    vector2 m_v2MouseDragDelta;
    vector2Int m_v2PrevMosuePos;
    float m_fPitch, m_fYaw;
};

END

