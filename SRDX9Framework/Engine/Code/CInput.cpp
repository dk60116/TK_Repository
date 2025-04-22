#include "CInput.h"
#include "CScreen.h"

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
    Release();
}

bool CInput::GetKey(_int _iKey)
{
    return m_bKeyState[_iKey] && GetForegroundWindow() == CScreen::GetInstance().getWindowHandle(L"Game");
}

bool CInput::GetKey_Editor(_int _iKey)
{
    return m_bKeyState[_iKey] && GetForegroundWindow() == CScreen::GetInstance().getWindowHandle(L"Scene");
}

bool CInput::GetKeyDown(_int _iKey)
{
    return m_bKeyState[_iKey] && !m_bPrevKeyState[_iKey] && GetForegroundWindow() == CScreen::GetInstance().getWindowHandle(L"Game");
}

bool CInput::GetKeyDown_Editor(_int _iKey)
{
    return m_bKeyState[_iKey] && !m_bPrevKeyState[_iKey] && GetForegroundWindow() == CScreen::GetInstance().getWindowHandle(L"Scene");
}

bool CInput::GetKeyUp(_int _iKey)
{
    return !m_bKeyState[_iKey] && m_bPrevKeyState[_iKey];
}

bool CInput::GetMouseButton(_int _button)
{
    if (_button == 0)
        _button = MOUSE_L;
    else if (_button == 1)
        _button = MOUSE_R;
    else
        return false;

    return m_bKeyState[_button] && GetForegroundWindow() == CScreen::GetInstance().getWindowHandle(L"Game");
}

bool CInput::GetMouseButton_Editor(_int _button)
{
    if (_button == 0)
        _button = MOUSE_L;
    else if (_button == 1)
        _button = MOUSE_R;
    else
        return false;

    return m_bKeyState[_button] && GetForegroundWindow() == CScreen::GetInstance().getWindowHandle(L"Scene");
}

bool CInput::GetMouseButtonDown(_int _button)
{
    if (_button == 0)
        _button = MOUSE_L;
    else if (_button == 1)
        _button = MOUSE_R;
    else
        return false;

    return m_bKeyState[_button] && !m_bPrevKeyState[_button] && GetForegroundWindow() == CScreen::GetInstance().getWindowHandle(L"Game");
}

bool CInput::GetMouseButtonDown_Editor(_int _button)
{
    if (_button == 0)
        _button = MOUSE_L;
    else if (_button == 1)
        _button = MOUSE_R;
    else
        return false;

    return m_bKeyState[_button] && !m_bPrevKeyState[_button] && GetForegroundWindow() == CScreen::GetInstance().getWindowHandle(L"Scene");
}

bool CInput::GetMouseButtonUp(_int _button)
{
    if (_button == 0)
        _button = MOUSE_L;
    else if (_button == 1)
        _button = MOUSE_R;
    else
        return false;

    return !m_bKeyState[_button] && m_bPrevKeyState[_button];
}

bool CInput::GetMouseButtonUp_Editor(_int _button)
{
    if (_button == 0)
        _button = MOUSE_L;
    else if (_button == 1)
        _button = MOUSE_R;
    else
        return false;

    return !m_bKeyState[_button] && m_bPrevKeyState[_button] && GetForegroundWindow() == CScreen::GetInstance().getWindowHandle(L"Scene");
}


const vector2Int CInput::GetMousePos()
{
    POINT ptMouse;
    GetCursorPos(&ptMouse);
    ScreenToClient(CScreen::GetInstance().getWindowHandle(L"Game"), &ptMouse);

    return vector2Int((int)ptMouse.x, (int)ptMouse.y);
}

//const int CInput::GetMouseWheelValue()
//{
//    return g_mouseWheelDelta;
//}

void CInput::Reset()
{
    //g_mouseWheelDelta = 0;

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
    //g_mouseWheelDelta = 0;

    for (auto& key : m_bKeyState)
    {
        m_bPrevKeyState[key.first] = key.second;
    }
}

void CInput::Release()
{
    m_bKeyState.clear();
    m_bPrevKeyState.clear();
}
