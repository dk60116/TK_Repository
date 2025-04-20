#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

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
    //const int GetMouseWheelValue();

public:
    void Reset();
    void Update();
    void LateUpdate();
    void Release();

private:
    static CInput* m_pInstance;
    map<int, bool> m_bKeyState;
    map<int, bool> m_bPrevKeyState;
};

END

