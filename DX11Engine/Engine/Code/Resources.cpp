#include "epch.h"
#include "Resources.h"

CResources::CResources()
	: m_strDefaultAssetPath(L"../Assets/")
	, m_strEngineFilePath(L"../EngineResource/")
{
}

CResources::~CResources()
{
	Release();
}

CResources& CResources::GetInstance()
{
	static CResources inst;
	return inst;
}

void CResources::Release()
{
}

_bool CResources::FileExists(wstring& _path)
{
	return fs::exists(_path) && fs::is_regular_file(_path);
}

_bool CResources::FileExists(string& _path)
{
	return fs::exists(_path) && fs::is_regular_file(_path);
}
