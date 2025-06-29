#pragma once

#include "epch.h"

NS_BEGIN(Engine)

class ENGINE_DLL CInput final
{
public:
    struct WheelAxisOption
    {
        float sensitivity = 1.f;
        float gravity = 5.f;
        float dead = 0.001f;
    };

	SINGLETONCLASS(CInput);

public:
    HRESULT Initialize();
    void Release();

public:
    _bool GetKey(_int _iKey);
    _bool GetKeyDown(_int _iKey);
    _bool GetKeyUp(_int _iKey);
    _bool GetMouseButton(_int _button);
    _bool GetMouseButtonDown(_int _button);
    _bool GetMouseButtonUp(_int _button);

    const vector2Int GetMousePos();
    const _float GetAxis(const wstring _axisName);
    const _float GetAxisRaw(const wstring _axisName);

public:
    _float& Get_WheelAxisRaw();

public:
    void Reset();
    void Update();

private:
    void InstallMouseHook();
    void UninstallMouseHook();

private:
    map<int, bool> m_bKeyState;
    map<int, bool> m_bPrevKeyState;
    WheelAxisOption m_sWheelOption;
    _float m_fWheelAxis;
    _float m_fWheelRaw;

public:
    static HHOOK s_mouseHook;
};

NS_END