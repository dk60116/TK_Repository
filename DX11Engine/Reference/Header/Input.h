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
    static HRESULT Initialize();
    static void Release();

public:
    static _bool GetKey_Editor(const _int _iKey);
    static _bool GetKeyDown_Editor(_int _iKey);
    static _bool GetKeyUp_Editor(_int _iKey);
    static _bool GetMouseButton_Editor(_int _button);
    static _bool GetMouseButtonDown_Editor(_int _button);
    static _bool GetMouseButtonUp_Editor(_int _button);

    static _bool GetKey(_int _iKey);
    static _bool GetKeyDown(_int _iKey);
    static _bool GetKeyUp(_int _iKey);
    static _bool GetMouseButton(_int _button);
    static _bool GetMouseButtonDown(_int _button);
    static _bool GetMouseButtonUp(_int _button);

    static vector2Int GetMousePos_Editor();
    static vector2Int GetMousePos();

    static _float GetAxis_Editor(const wstring _axisName);
    static _float GetAxisRaw_Editor(const wstring _axisName);

    static _float GetAxis(const wstring _axisName);
    static float GetAxisRaw(const wstring _axisName);

public:
    static _float& Get_WheelAxisRaw();

public:
    static void Reset();
    static void Update();

private:
    void InstallMouseHook();
    void UninstallMouseHook();

private:
    static _bool IsEditor();

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