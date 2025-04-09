#pragma once

#include "Engine_Define.h"
#include <iostream>
#include "string"

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
    static void Print(const int format, ...);
    static void Print(const float format, ...);
    static void Print(const vector3 format, ...);

public:
    static string WStringToString(const std::wstring& wstr);
};

END

