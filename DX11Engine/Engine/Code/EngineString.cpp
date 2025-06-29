#include "epch.h"
#include "EngineString.h"

string CEngineString::WStringToString(const wstring& _wstr)
{
    if (_wstr.empty())
        return string();

    _int size_needed = WideCharToMultiByte(CP_UTF8, 0, &_wstr[0], (int)_wstr.size(), NULL, 0, NULL, NULL);
    string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &_wstr[0], (int)_wstr.size(), &str[0], size_needed, NULL, NULL);
    return str;
}

wstring CEngineString::StringToWString(const string& _str)
{
    if (_str.empty())
        return wstring();

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, _str.c_str(), (int)_str.size(), NULL, 0);
    wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, _str.c_str(), (int)_str.size(), &wstr[0], size_needed);

    return wstr;
}
