#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

struct WheelAxisOption
{
    float sensitivity = 1.f;
    float gravity = 5.f;
    float dead = 0.001f;
};

class ENGINE_DLL CInput
{
	SINGLETONCLASS(CInput);

public:
    bool GetKey(_int _iKey);
    bool GetKey_Editor(_int _iKey);
    bool GetKeyDown(_int _iKey);
    bool GetKeyDown_Editor(_int _iKey);
    bool GetKeyUp(_int _iKey);
    bool GetMouseButton(_int _button);
    bool GetMouseButton_Editor(_int _button);
    bool GetMouseButtonDown(_int _button);
    bool GetMouseButtonDown_Editor(_int _button);
    bool GetMouseButtonUp(_int _button);
    bool GetMouseButtonUp_Editor(_int button);

    const vector2Int GetMousePos();
    const _float GetAxis(const wstring _axisName);
    const _float GetAxis_Editor(const wstring _axisName);
    const _float GetAxisRaw(const wstring _axisName);
    const _float GetAxisRaw_Editor(const wstring _axisName);

    void OnMouseWheel(WPARAM _wParam);

public:
    void Reset();
    void Update();
    void LateUpdate();
    void Release();

private:
    static CInput* m_pInstance;
    map<int, bool> m_bKeyState;
    map<int, bool> m_bPrevKeyState;
    WheelAxisOption m_sWheelOption;
    _float m_fWheelAxis;
    _float m_fWheelRaw;
};

END

