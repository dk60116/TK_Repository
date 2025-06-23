#include "epch.h"
#include "EngineDebug.h"
#include <iostream>
#include <cstdio>
#include <cstdarg>

#ifdef _DEBUG

CDebug::CDebug()
{
}

CDebug::~CDebug()
{
}

CDebug& CDebug::GetInstance()
{
    static CDebug inst;
    return inst;
}

HRESULT CDebug::Initialize()
{
    AllocConsole();

    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);

    cout << "Game Started\n";

    return S_OK;
}

void CDebug::Release()
{
    FreeConsole();
}

void CDebug::Log(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
    fflush(stdout);
}

void CDebug::Log(const string format, ...)
{
    va_list args;
    va_start(args, format);

    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format.c_str(), args);

    va_end(args);

    printf("%s\n", buffer);
    fflush(stdout);
}

void CDebug::Log(const wstring format, ...)
{
    Log(WStringToString(format));
}

void CDebug::Log(const int format, ...)
{
    Log(to_string(format));
}

void CDebug::Log(const float format, ...)
{
    Log(to_string(format));
}

void CDebug::Log(const vector2 format, ...)
{
    Log("vector2(" + to_string(format.x) + ", " + to_string(format.y) + ')');
}

void CDebug::Log(const vector2Int format, ...)
{
    Log("vector2Int(" + to_string(format.x) + ", " + to_string(format.y) + ')');
}

void CDebug::Log(const vector3 format, ...)
{
    Log("vector3(" + to_string(format.x) + ", " + to_string(format.y) + ", " + to_string(format.z) + ')');
}

string CDebug::WStringToString(const std::wstring& wstr)
{
    if (wstr.empty()) return string();

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str[0], size_needed, NULL, NULL);
    return str;
}

void CDebug::LogError(const char* format, ...)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    WORD saved_attributes = consoleInfo.wAttributes;

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");
    fflush(stdout);

    SetConsoleTextAttribute(hConsole, saved_attributes);
}

void CDebug::LogError(const string format, ...)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    WORD saved_attributes = consoleInfo.wAttributes;

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);

    va_list args;
    va_start(args, format);

    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format.c_str(), args);

    va_end(args);

    printf("%s\n", buffer);
    fflush(stdout);

    SetConsoleTextAttribute(hConsole, saved_attributes);
}

void CDebug::LogError(const wstring format, ...)
{
    LogError(WStringToString(format));
}

void CDebug::LogError(const int format, ...)
{
    LogError(to_string(format));
}

void CDebug::LogError(const float format, ...)
{
    LogError(to_string(format));
}

void CDebug::LogError(const vector2 format, ...)
{
    LogError("vector2(" + to_string(format.x) + ", " + to_string(format.y) + ')');
}

void CDebug::LogError(const vector2Int format, ...)
{
    LogError("vector2Int(" + to_string(format.x) + ", " + to_string(format.y) + ')');
}

void CDebug::LogError(const vector3 format, ...)
{
    LogError("vector3(" + to_string(format.x) + ", " + to_string(format.y) + ", " + to_string(format.z) + ')');
}

#else
CDebug::CDebug() {}
CDebug::~CDebug() {}
CDebug& CDebug::GetInstance() { static CDebug inst; return inst; }
HRESULT CDebug::Initialize() { return S_OK; }
void CDebug::Release() {}
void CDebug::Log(const char*, ...) {}
void CDebug::Log(const std::string format, ...) {}
void CDebug::Log(const wstring format, ...) {}
void CDebug::Log(const int foramt, ...) {}
void CDebug::Log(const float format, ...) {}
void CDebug::Log(const vector3 format, ...) {}
void CDebug::Log(const vector2Int format, ...) {}
void CDebug::LogError(const char* format, ...) {}
void CDebug::LogError(const string format, ...) {}
void CDebug::LogError(const wstring format, ...) {}
void CDebug::LogError(const int format, ...) {}
void CDebug::LogError(const float format, ...) {}
void CDebug::LogError(const vector2 format, ...) {}
void CDebug::LogError(const vector2Int format, ...) {}
void CDebug::LogError(const vector3 format, ...) {}
#endif
