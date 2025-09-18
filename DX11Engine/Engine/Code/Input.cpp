#include "epch.h"
#include "Input.h"

HHOOK CInput::s_mouseHook = nullptr;

CInput::CInput()
    : m_fWheelAxis(0.f)
    , m_fWheelRaw(0.f)
    , m_vMouseRawDelta(vector2::zero())
{
}

CInput::~CInput()
{
    Release();
}

CInput& CInput::GetInstance()
{
    static CInput inst;
    return inst;
}

HRESULT CInput::Initialize()
{
    GetInstance().InstallMouseHook();

    RegiserMouseRaw();

    return S_OK;
}

void CInput::Release()
{
    GetInstance().m_bKeyState.clear();
    GetInstance().m_bPrevKeyState.clear();

    GetInstance().UninstallMouseHook();
}

_bool CInput::GetKey_Editor(_int _iKey)
{
    return GetInstance().m_bKeyState[_iKey] && GetInstance().IsEditor();
}

_bool CInput::GetKeyDown_Editor(_int _iKey)
{
    return GetInstance().m_bKeyState[_iKey] && !GetInstance().m_bPrevKeyState[_iKey] && GetInstance().IsEditor();
}

_bool CInput::GetKeyUp_Editor(_int _iKey)
{
    return !GetInstance().m_bKeyState[_iKey] && GetInstance().m_bPrevKeyState[_iKey] && GetInstance().IsEditor();
}

_bool CInput::GetMouseButton_Editor(_int _button)
{
    if (_button == 0)
        _button = MOUSE_L;
    else if (_button == 1)
        _button = MOUSE_R;
    else if (_button == 2)
        _button = MOUSE_WHILL;
    else
        return false;

    return GetInstance().m_bKeyState[_button] && GetInstance().IsEditor();
}

_bool CInput::GetMouseButtonDown_Editor(_int _button)
{
    if (_button == 0)
        _button = MOUSE_L;
    else if (_button == 1)
        _button = MOUSE_R;
    else if (_button == 2)
        _button = MOUSE_WHILL;
    else
        return false;

    return GetInstance().m_bKeyState[_button] && !GetInstance().m_bPrevKeyState[_button] && GetInstance().IsEditor();
}

_bool CInput::GetMouseButtonUp_Editor(_int _button)
{
    if (_button == 0)
        _button = MOUSE_L;
    else if (_button == 1)
        _button = MOUSE_R;
    else if (_button == 2)
        _button = MOUSE_WHILL;
    else
        return false;

    return !GetInstance().m_bKeyState[_button] && GetInstance().m_bPrevKeyState[_button] && GetInstance().IsEditor();
}

bool CInput::GetKey(const _int _iKey)
{
    return GetInstance().m_bKeyState[_iKey] && !GetInstance().IsEditor();
}

bool CInput::GetKeyDown(_int _iKey)
{
    return GetInstance().m_bKeyState[_iKey] && !GetInstance().m_bPrevKeyState[_iKey] && !GetInstance().IsEditor();
}

bool CInput::GetKeyUp(_int _iKey)
{
    return !GetInstance().m_bKeyState[_iKey] && GetInstance().m_bPrevKeyState[_iKey] && !GetInstance().IsEditor();
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

    return GetInstance().m_bKeyState[_button] && !GetInstance().IsEditor();
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

    return GetInstance().m_bKeyState[_button] &&!GetInstance().m_bPrevKeyState[_button] && !GetInstance().IsEditor();
}

bool CInput::GetMouseButtonUp(_int _button)
{
    if (_button == 0)
        _button = MOUSE_L;
    else if (_button == 1)
        _button = MOUSE_R;
    else if (_button == 2)
        _button = MOUSE_WHILL;
    else
        return false;

    return !GetInstance().m_bKeyState[_button] && GetInstance().m_bPrevKeyState[_button] && !GetInstance().IsEditor();
}

vector2Int CInput::GetMousePos_Editor()
{
    POINT ptMouse;
    GetCursorPos(&ptMouse);

    ScreenToClient(CEditor::Get_EditorWindow(), &ptMouse);

    ptMouse.y -= CEditor::Get_Options().topBarHeight;

    return vector2Int((_int)ptMouse.x, (_int)ptMouse.y);
}

vector2Int CInput::GetMousePos()
{
    POINT ptMouse;
    GetCursorPos(&ptMouse);
    ScreenToClient(CDisplay::Get_GameWindow(), &ptMouse);

    return vector2Int((_int)ptMouse.x, (_int)ptMouse.y);
}

_float CInput::GetAxis_Editor(const wstring& _axisName)
{
    if (!IsEditor())
        return 0.f;

    _float result = 0.f;

    if (_axisName == L"Horizontal")
    {
        if (GetKey_Editor(LEFT))
            result -= 1.f;
        if (GetKey_Editor(RIGHT))
            result += 1.f;
        if (GetKey_Editor(A))
            result -= 1.f;
        if (GetKey_Editor(D))
            result += 1.f;
    }
    else if (_axisName == L"Vertical")
    {
        if (GetKey_Editor(DOWN))
            result -= 1.f;
        if (GetKey_Editor(UP))
            result += 1.f;
        if (GetKey_Editor(S))
            result -= 1.f;
        if (GetKey_Editor(W))
            result += 1.f;
    }
    else if (_axisName == L"Mouse ScrollWheel")
        result = GetInstance().m_fWheelAxis;
    else
        return 0.f;

    result = clamp(result, -1.f, 1.f);

    return result;
}

_float CInput::GetAxis(const wstring& _axisName)
{
    if (IsEditor())
        return 0.f;

    _float result = 0.f;

    if (_axisName == L"Horizontal")
    {
        if (GetKey(LEFT))
            result -= 1.f;
        if (GetKey(RIGHT))
            result += 1.f;
        if (GetKey(A))
            result -= 1.f;
        if (GetKey(D))
            result += 1.f;
    }
    else if (_axisName == L"Vertical")
    {
        if (GetKey(DOWN))
            result -= 1.f;
        if (GetKey(UP))
            result += 1.f;
        if (GetKey(S))
            result -= 1.f;
        if (GetKey(W))
            result += 1.f;
    }
    else if (_axisName == L"Mosue MoveDelta X")
        result = GetInstance().m_vMouseRawDelta.x;
    else if (_axisName == L"Mosue MoveDelta Y")
        result = GetInstance().m_vMouseRawDelta.y;
    else if (_axisName == L"Mouse ScrollWheel")
        result = GetInstance().m_fWheelAxis;
    else
        return 0.f;

    result = clamp(result, -1.f, 1.f);

    return result;
}

float CInput::GetAxisRaw(const wstring& _axisName)
{
    _float result = 0.f;

    const _float delta = GetInstance().GetAxis(_axisName);

    if (delta < 0.f)
        result = -1.f;
    else if (delta > 0.f)
        result = 1.f;

    return result;
}

_float& CInput::Get_WheelAxisRaw()
{
    return GetInstance().m_fWheelRaw;
}

void CInput::Reset()
{
    for (auto& key : GetInstance().m_bKeyState)
    {
        key.second = false;
        GetInstance().m_bPrevKeyState[key.first] = false;
    }

    GetInstance().m_vMouseRawDelta = vector2::zero();
}

void CInput::Update()
{
    for (auto& key : GetInstance().m_bKeyState)
    {
        GetInstance().m_bPrevKeyState[key.first] = key.second;
        key.second = (GetAsyncKeyState(key.first) & 0x8000) != 0;
    }

    if (abs(GetInstance().m_fWheelRaw) > 0.f)
    {
        GetInstance().m_fWheelAxis += GetInstance().m_fWheelRaw * GetInstance().m_sWheelOption.sensitivity;
        GetInstance().m_fWheelRaw = 0.f;
    }

    if (GetInstance().m_fWheelAxis > 0.f)
        GetInstance().m_fWheelAxis = max(0.f, GetInstance().m_fWheelAxis - GetInstance().m_sWheelOption.gravity * DELTA_TIME);
    else if (GetInstance().m_fWheelAxis < 0.f)
        GetInstance().m_fWheelAxis = min(0.f, GetInstance().m_fWheelAxis + GetInstance().m_sWheelOption.gravity * DELTA_TIME);

    if (abs(GetInstance().m_fWheelAxis) < GetInstance().m_sWheelOption.dead)
        GetInstance().m_fWheelAxis = 0.f;

    GetInstance().m_fWheelAxis = clamp(GetInstance().m_fWheelAxis, -1.f, 1.f);
}

void CInput::LateUpdate()
{
    GetInstance().m_vMouseRawDelta = vector2::zero();
}

LRESULT MouseProc(_int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        if (wParam == WM_MOUSEWHEEL)
        {
            MSLLHOOKSTRUCT* pMouse = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
            if (pMouse)
            {
                short delta = static_cast<short>(HIWORD(pMouse->mouseData));
                _float normalized = static_cast<_float>(delta) / WHEEL_DELTA;

                CInput::Get_WheelAxisRaw() += normalized;
            }
        }
    }

    return CallNextHookEx(CInput::s_mouseHook, nCode, wParam, lParam);
}

void CInput::InstallMouseHook()
{
    if (s_mouseHook == nullptr)
        s_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, nullptr, 0);
}

void CInput::UninstallMouseHook()
{
    if (s_mouseHook != nullptr)
    {
        UnhookWindowsHookEx(s_mouseHook);
        s_mouseHook = nullptr;
    }
}

void CInput::RegiserMouseRaw_Editor()
{
}

void CInput::RegiserMouseRaw()
{
    RAWINPUTDEVICE rid{};
    rid.usUsagePage = 0x01;
    rid.usUsage = 0x02; 
    rid.dwFlags = RIDEV_INPUTSINK;
    rid.hwndTarget = CDisplay::GetInstance().Get_GameWindow();
    RegisterRawInputDevices(&rid, 1, sizeof(rid));
}

void CInput::AddRawMouseDelta(_float _x, _float _y)
{
    GetInstance().m_vMouseRawDelta.x += _x;
    GetInstance().m_vMouseRawDelta.y += _y;
}

_bool CInput::IsEditor()
{
#ifdef _CLIENT_BUILD
    return false;
#endif

    return GetForegroundWindow() == CEditor::Get_EditorWindow();
}
