#pragma once

#include "Engine_Define.h"
#include <iostream>

using namespace std;

BEGIN(Engine)

class ENGINE_DLL CDebug
{
public:
    static void Init();
    static void Release();
    static void Print(const char* format, ...);
    static void Print(const string format, ...);
    static void Print(const wstring format, ...);

public:
    static string WStringToString(const std::wstring& wstr);
};

END

