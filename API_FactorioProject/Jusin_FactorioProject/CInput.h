#pragma once

#include "pch.h"

enum KEY_CODE
{
    KEY_START = 0,

    ZERO = 0x30, ONE = 0x31, TWO = 0x32, THREE = 0x33, FOUR = 0x34, FIVE = 0x35, SIX = 0x36, SEVEN = 0x37, EIGHT = 0x38, NINE = 0x39,

    Q = 'Q', W = 'W', E = 'E', R = 'R', T = 'T', Y = 'Y', U = 'U', I = 'I', O = 'O', P = 'P',
    A = 'A', S = 'S', D = 'D', F = 'F', G = 'G', H = 'H', J = 'J', K = 'K', L = 'L',
    Z = 'Z', X = 'X', C = 'C', V = 'V', B = 'B', N = 'N', M = 'M',

    SHIFT = VK_SHIFT, L_SHIFT = VK_LSHIFT, R_SHIFT = VK_RSHIFT, CONTROL = VK_CONTROL, L_CONTROL = VK_LCONTROL, R_CONTROL = VK_RCONTROL,
    ALT = VK_MENU, L_ALT = VK_LMENU, R_ALT = VK_RMENU,

    RETURN = VK_RETURN, ESCAPE = VK_ESCAPE, SPACE = VK_SPACE, TAB = VK_TAB,

    ARROW_UP = VK_UP, ARROW_DOWN = VK_DOWN, ARROW_LEFT = VK_LEFT, ARROW_RIGHT = VK_RIGHT,

    MOUSE_L = VK_LBUTTON, MOUSE_R = VK_RBUTTON, MOUSEWHILL = WM_MOUSEWHEEL,

    KEY_END
};

class CInput
{
    SingletonClass(CInput);

public:
    bool GetKey(int _iKey);
    bool GetKeyDown(int _iKey);
    bool GetKeyUp(int _iKey);
    bool GetMouseButton(int _button);
    bool GetMouseButtonDown(int _button);
    bool GetMouseButtonUp(int button);

    const vector2Int GetMousePos();
    const int GetMouseWheelValue();

public:
    void Reset();
    void Update();
    void LateUpdate();

private:
    static CInput* m_pInstance;
    map<int, bool> m_bKeyState;
    map<int, bool> m_bPrevKeyState;
};
