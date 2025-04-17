#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CInput
{
	SINGLETONCLASS(CInput);

public:
    bool GetKey(int _iKey);
    bool GetKeyDown(int _iKey);
    bool GetKeyUp(int _iKey);
    bool GetMouseButton(int _button);
    bool GetMouseButtonDown(int _button);
    bool GetMouseButtonUp(int button);

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

