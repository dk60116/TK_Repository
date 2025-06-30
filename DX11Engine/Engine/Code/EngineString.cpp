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

string CEngineString::Trim(const string& str)
{
    const char* whitespace = " \t\n\r";

    const size_t start = str.find_first_not_of(whitespace);

    if (start == string::npos)
        return "";

    const size_t end = str.find_last_not_of(whitespace);
    
    return str.substr(start, end - start + 1);
}

wstring CEngineString::Trim(const wstring& wstr)
{
    const wchar_t* whitespace = L" \t\n\r";

    const size_t start = wstr.find_first_not_of(whitespace);

    if (start == wstring::npos)
        return L"";

    const size_t end = wstr.find_last_not_of(whitespace);
    
    return wstr.substr(start, end - start + 1);
}
