#include "Input.h"
#include "CEngineEditor.h"
#include "CDebug.h"

CInput::CInput()
    : m_fWheelAxis(0.f)
    , m_fWheelRaw(0.f)
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
    return m_bKeyState[_iKey] && GetForegroundWindow() == CEngineEditor::GetInstance().FindWindowHandle(L"Game");
}

bool CInput::GetKey_Editor(_int _iKey)
{
    return m_bKeyState[_iKey] && GetForegroundWindow() == CEngineEditor::GetInstance().FindWindowHandle(L"Scene");
}

bool CInput::GetKeyDown(_int _iKey)
{
    return m_bKeyState[_iKey] && !m_bPrevKeyState[_iKey] && GetForegroundWindow() == CEngineEditor::GetInstance().FindWindowHandle(L"Game");
}

bool CInput::GetKeyDown_Editor(_int _iKey)
{
    return m_bKeyState[_iKey] && !m_bPrevKeyState[_iKey] && 
        GetForegroundWindow() != CEngineEditor::GetInstance().FindWindowHandle(L"Game");
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
    else if (_button == 2)
        _button = MOUSE_WHILL;
    else
        return false;

    return m_bKeyState[_button] && GetForegroundWindow() == CEngineEditor::GetInstance().FindWindowHandle(L"Game");
}

bool CInput::GetMouseButton_Editor(_int _button)
{
    if (_button == 0)
        _button = MOUSE_L;
    else if (_button == 1)
        _button = MOUSE_R;
    else if (_button == 2)
        _button = MOUSE_WHILL;
    else
        return false;

    return m_bKeyState[_button] && GetForegroundWindow() == CEngineEditor::GetInstance().FindWindowHandle(L"Scene");
}

bool CInput::GetMouseButtonDown(_int _button)
{
    if (_button == 0)
        _button = MOUSE_L;
    else if (_button == 1)
        _button = MOUSE_R;
    else if (_button == 2)
        _button = MOUSE_WHILL;
    else
        return false;

    return m_bKeyState[_button] && !m_bPrevKeyState[_button] && GetForegroundWindow() == CEngineEditor::GetInstance().FindWindowHandle(L"Game");
}

bool CInput::GetMouseButtonDown_Editor(_int _button)
{
    if (_button == 0)
        _button = MOUSE_L;
    else if (_button == 1)
        _button = MOUSE_R;
    else if (_button == 2)
        _button = MOUSE_WHILL;
    else
        return false;

    return m_bKeyState[_button] && !m_bPrevKeyState[_button] && GetForegroundWindow() == CEngineEditor::GetInstance().FindWindowHandle(L"Scene");
}

bool CInput::GetMouseButtonUp(_int _button)
{
    if (_button == 0)
        _button = MOUSE_L;
    else if (_button == 1)
        _button = MOUSE_R;
    else if (_button == 3)
        _button = MOUSE_WHILL;
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
    else if (_button == 2)
        _button = MOUSE_WHILL;
    else
        return false;

    return !m_bKeyState[_button] && m_bPrevKeyState[_button] && GetForegroundWindow() == CEngineEditor::GetInstance().FindWindowHandle(L"Scene");
}


const vector2Int CInput::GetMousePos()
{
    POINT ptMouse;
    GetCursorPos(&ptMouse);
    ScreenToClient(CEngineEditor::GetInstance().FindWindowHandle(L"Game"), &ptMouse);

    return vector2Int((int)ptMouse.x, (int)ptMouse.y);
}

const _float CInput::GetAxis(const wstring _axisName)
{
    float result = 0.f;

    if (_axisName == L"Horizontal")
    {
        if (GetKeyDown(LEFT))
            result -= 1.f;
        if (GetKeyDown(RIGHT))
            result += 1.f;
        if (GetKeyDown(A))
            result -= 1.f;
        if (GetKeyDown(D))
            result += 1.f;
    }
    else if (_axisName == L"Vertical")
    {
        if (GetKeyDown(DOWN))
            result -= 1.f;
        if (GetKeyDown(UP))
            result += 1.f;
        if (GetKeyDown(S))
            result -= 1.f;
        if (GetKeyDown(W))
            result += 1.f;
    }
    else if (_axisName == L"Mouse ScrollWheel")
    {
        result = m_fWheelAxis;
    }
    else
        return 0.f;

    result = clamp(result, -1.f, 1.f);

    return result;
}

const _float CInput::GetAxis_Editor(const wstring _axisName)
{
    float result = 0.f;

    if (_axisName == L"Horizontal")
    {
        if (GetKeyDown_Editor(LEFT))
            result -= 1.f;
        if (GetKeyDown_Editor(RIGHT))
            result += 1.f;
        if (GetKeyDown_Editor(A))
            result -= 1.f;
        if (GetKeyDown_Editor(D))
            result += 1.f;
    }
    else if (_axisName == L"Vertical")
    {
        if (GetKeyDown_Editor(DOWN))
            result -= 1.f;
        if (GetKeyDown_Editor(UP))
            result += 1.f;
        if (GetKeyDown_Editor(S))
            result -= 1.f;
        if (GetKeyDown_Editor(W))
            result += 1.f;
    }
    else if (_axisName == L"Mouse ScrollWheel")
    {
        result = m_fWheelAxis;
    }
    else
        return 0.f;

    result = clamp(result, -1.f, 1.f);

    return GetForegroundWindow() == CEngineEditor::GetInstance().FindWindowHandle(L"Scene") ? result : 0;
}

void CInput::OnMouseWheel(WPARAM _wParam)
{
    m_fWheelRaw += static_cast<float>(GET_WHEEL_DELTA_WPARAM(_wParam)) / WHEEL_DELTA;
}

void CInput::Reset()
{
    for (auto& key : m_bKeyState)
    {
        key.second = false;
        m_bPrevKeyState[key.first] = false;
    }
}

void CInput::Update()
{
    float dt = CTime::GetInstance().Get_DeltaTime();

    if (abs(m_fWheelRaw) > 0.f)
    {
        m_fWheelAxis += m_fWheelRaw * m_sWheelOption.sensitivity;
        m_fWheelRaw = 0.f;
    }
    
    if (m_fWheelAxis > 0.f)
        m_fWheelAxis = max(0.f, m_fWheelAxis - m_sWheelOption.gravity * dt);
    else if (m_fWheelAxis < 0.f)
        m_fWheelAxis = min(0.f, m_fWheelAxis + m_sWheelOption.gravity * dt);

    if (abs(m_fWheelAxis) < m_sWheelOption.dead)
        m_fWheelAxis = 0.f;

    m_fWheelAxis = clamp(m_fWheelAxis, -1.f, 1.f);

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
    Reset();
}

void CInput::Release()
{
    m_bKeyState.clear();
    m_bPrevKeyState.clear();
}
