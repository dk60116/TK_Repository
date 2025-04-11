#include "CConsole.h"
#include <windows.h>
#include <cstdio>
#include <cstdarg>
#include "CMonoBehaviour.h"

using namespace std;

#ifdef _DEBUG
void CConsole::Init()
{
    AllocConsole();

    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);

    std::cout << "Game Started\n";
}

void CConsole::Release()
{
    FreeConsole();
}

void CConsole::Print(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
    fflush(stdout);
}

void CConsole::Print(const string format, ...)
{
    va_list args;
    va_start(args, format);

    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format.c_str(), args);

    va_end(args);

    printf("%s\n", buffer);
    fflush(stdout);
}


void CConsole::Print(const wstring format, ...)
{
    Print(CMonoBehaviour::WStringToString(format));
}

void CConsole::Print(const int format, ...)
{
    Print(to_string(format));
}

void CConsole::Print(const float format, ...)
{
    Print(to_string(format));
}

void CConsole::Print(const vector2 format, ...)
{
    Print('(' + to_string(format.x) + ", " + to_string(format.y) + ')');
}

void CConsole::Print(const vector2Int format, ...)
{
    Print('(' + to_string(format.x) + ", " + to_string(format.y) + ')');
}

#else
void CConsole::Init() {}
void CConsole::Release() {}
void CConsole::Print(const char*, ...) {}
void CConsole::Print(const std::string format, ...) {}
void CConsole::Print(const wstring format, ...) {}
void CConsole::Print(const int format, ...) {}
void CConsole::Print(const float format, ...) {}
void CConsole::Print(const vector2 format, ...) {}
void CConsole::Print(const vector2Int format, ...) {}
#endif
