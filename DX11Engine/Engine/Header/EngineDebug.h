#pragma once

#include "epch.h"

NS_BEGIN(Engine)

class ENGINE_DLL CDebug final 
{
    SINGLETONCLASS(CDebug);

public:
    static HRESULT Initialize();
    void Release();

public:
    static void Log(const char* format, ...);
    static void Log(const string format, ...);
    static void Log(const wstring& format, ...);
    static void Log(const _bool format, ...);
    static void Log(const _int format, ...);
    static void Log(const _uint format, ...);
    static void Log(const size_t format, ...);
    static void Log(const _float format, ...);
    static void Log(const vector2 format, ...);
    static void Log(const vector2Int format, ...);
    static void Log(const vector3 format, ...);
    static void Log(const quaternion format, ...);
    static void Log(const _float4x4 format, ...);
    static void Log(const _matrix, ...);

    static void LogError(const char* format, ...);
    static void LogError(const string format, ...);
    static void LogError(const wstring& format, ...);
    static void LogError(const _bool format, ...);
    static void LogError(const _int format, ...);
    static void LogError(const _uint format, ...);
    static void LogError(const size_t format, ...);
    static void LogError(const _float format, ...);
    static void LogError(const vector2 format, ...);
    static void LogError(const vector2Int format, ...);
    static void LogError(const vector3 format, ...);
	static void LogError(const quaternion format, ...);

    static void LogWarning(const char* format, ...);
    static void LogWarning(const string format, ...);
    static void LogWarning(const wstring& format, ...);
    static void LogWarning(const _bool format, ...);
    static void LogWarning(const _int format, ...);
    static void LogWarning(const _uint format, ...);
    static void LogWarning(const size_t format, ...);
    static void LogWarning(const _float format, ...);
    static void LogWarning(const vector2 format, ...);
    static void LogWarning(const vector2Int format, ...);
    static void LogWarning(const vector3 format, ...);
	static void LogWarning(const quaternion format, ...);

    static string MemoryUseLog();

private:
    static void ShowElapsedTime();
};

NS_END

