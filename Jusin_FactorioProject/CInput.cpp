#include "CInput.h"
#include "Jusin_FactorioProject.h"

extern HWND g_hWnd;
extern int g_mouseWheelDelta;

CInput::CInput()
{
    for (int i = 0; i < KEY_END; ++i)
    {
        m_bKeyState[(KEY_CODE)i] = false;
        m_bPrevKeyState[(KEY_CODE)i] = false;
    }

    m_bKeyState[MOUSE_L] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
    m_bKeyState[MOUSE_R] = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
}

CInput::~CInput()
{
    m_bKeyState.clear();
    m_bPrevKeyState.clear();
}

bool CInput::GetKey(int _iKey)
{
    return m_bKeyState[_iKey];
}

bool CInput::GetKeyDown(int _iKey)
{
    return m_bKeyState[_iKey] && !m_bPrevKeyState[_iKey];
}

bool CInput::GetKeyUp(int _iKey)
{
    return !m_bKeyState[_iKey] && m_bPrevKeyState[_iKey];
}

bool CInput::GetMouseButton(int _button)
{
    if (_button == 0)
        _button = MOUSE_L;
    else if (_button == 1)
        _button = MOUSE_R;
    else
        return false;

    return m_bKeyState[_button];
}

bool CInput::GetMouseButtonDown(int _button)
{
    if (_button == 0)
        _button = MOUSE_L;
    else if (_button == 1)
        _button = MOUSE_R;
    else
        return false;

    return m_bKeyState[_button] && !m_bPrevKeyState[_button];
}

bool CInput::GetMouseButtonUp(int _button)
{
    if (_button == 0)
        _button = MOUSE_L;
    else if (_button == 1)
        _button = MOUSE_R;
    else
        return false;

    return !m_bKeyState[_button] && m_bPrevKeyState[_button];
}


const vector2Int CInput::GetMousePos()
{
    POINT ptMouse;
    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    return vector2Int((int)ptMouse.x, (int)ptMouse.y);
}

const int CInput::GetMouseWheelValue()
{
    return g_mouseWheelDelta;
}

void CInput::Reset()
{
    g_mouseWheelDelta = 0;

    for (auto& key : m_bKeyState)
    {
        key.second = false;
        m_bPrevKeyState[key.first] = false;
    }
}

void CInput::Update()
{
    for (auto& key : m_bKeyState)
    {
        bool newState = (GetAsyncKeyState(key.first) & 0x8000) != 0;

        if (newState != key.second)
        {
            key.second = newState;
        }
    }
}

void CInput::LateUpdate()
{
    g_mouseWheelDelta = 0;

    for (auto& key : m_bKeyState)
    {
        m_bPrevKeyState[key.first] = key.second;
    }
}