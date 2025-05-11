#pragma once

#include <iostream>
#include "struct.h"

using namespace std;

class CConsole
{
public:
    static void Init();
    static void Release();
    static void Print(const char* format, ...);
    static void Print(const string format, ...);
    static void Print(const wstring format, ...);
    static void Print(const int format, ...);
    static void Print(const float format, ...);
    static void Print(const vector2 format, ...);
    static void Print(const vector2Int format, ...);
};
