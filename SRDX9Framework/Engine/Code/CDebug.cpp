#include "CDebug.h"
#include <windows.h>
#include <cstdio>
#include <cstdarg>

#ifdef _DEBUG

void CDebug::Init()
{
    AllocConsole();

    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);

    cout << "Game Started\n";
}

void CDebug::Release()
{
    FreeConsole();
}

void CDebug::Print(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
    fflush(stdout);
}

void CDebug::Print(const string format, ...)
{
    va_list args;
    va_start(args, format);

    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format.c_str(), args);

    va_end(args);

    printf("%s\n", buffer);
    fflush(stdout);
}


void CDebug::Print(const wstring format, ...)
{
    Print(WStringToString(format));
}

void CDebug::Print(const int format, ...)
{
    Print(to_string(format));
}

void CDebug::Print(const float format, ...)
{
    Print(to_string(format));
}

void CDebug::Print(const vector3 format, ...)
{
    Print("vector3(" + to_string(format.x) + ", " + to_string(format.y) + ", " + to_string(format.z) + ')');
}

string CDebug::WStringToString(const std::wstring& wstr)
{
    if (wstr.empty()) return string();

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str[0], size_needed, NULL, NULL);
    return str;
}

#else
void CDebug::Init() {}
void CDebug::Release() {}
void CDebug::Print(const char*, ...) {}
void CDebug::Print(const std::string format, ...) {}
void CDebug::Print(const wstring format, ...) {}
void CDebug::Print(const int foramt, ...) {}
void CDebug::Print(const float format, ...) {}
void CDebug::Print(const vector3 format, ...) {}
 #endif
