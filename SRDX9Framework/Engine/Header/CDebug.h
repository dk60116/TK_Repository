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

public:
    static void Log(const char* format, ...);
    static void Log(const string format, ...);
    static void Log(const wstring format, ...);
    static void Log(const int format, ...);
    static void Log(const float format, ...);
    static void Log(const vector2 format, ...);
    static void Log(const vector2Int format, ...);
    static void Log(const vector3 format, ...);

    static void LogError(const char* format, ...);
    static void LogError(const string format, ...);
    static void LogError(const wstring format, ...);
    static void LogError(const int format, ...);
    static void LogError(const float format, ...);
    static void LogError(const vector2 format, ...);
    static void LogError(const vector2Int format, ...);
    static void LogError(const vector3 format, ...);

public:
    static string WStringToString(const std::wstring& wstr);
};

END

