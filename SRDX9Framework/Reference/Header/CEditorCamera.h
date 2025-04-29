#pragma once

#include "CCamera.h"

BEGIN(Engine)

class ENGINE_DLL CEditorCamera final
    : public CCamera 
{
private:
    CEditorCamera();
    ~CEditorCamera();

public:
    static CEditorCamera* Create();
    void AwakeEditor();
    void UpdateEditor() override;
    void GotoViewGameObject(CGameObject* _gameObject);

private:
    float m_fMoveSpeed, m_fRotateSpeed;
    float m_fCrtMoveSpd;
    _bool m_bRMouseDowned;
    vector2 m_v2MouseDragDelta;
    vector2Int m_v2PrevMosuePos;
    float m_fPitch, m_fYaw;
    _bool m_bMoving;
    float m_fMoveTimeTotal, m_fMoveTimeCur;
    vector3 m_v3MoveFrom, m_v3MoveTo;
    _bool m_bZoomDistToggle;
};

END

