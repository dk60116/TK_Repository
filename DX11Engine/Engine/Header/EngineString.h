#pragma once

#include "epch.h"

class ENGINE_DLL CEngineString
{
	SINGLETONCLASS(CEngineString);

public:
	static string WStringToString(const wstring& _wstr);
	static wstring StringToWString(const string& _str);
	static string Trim(const string& str);
	static wstring Trim(const wstring& wstr);
};

