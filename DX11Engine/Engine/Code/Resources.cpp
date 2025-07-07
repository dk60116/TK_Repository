#include "epch.h"
#include "Resources.h"

CResources::CResources()
	: m_strDefaultAssetPath(L"../Assets/")
	, m_strEngineFilePath(L"../EngineResource/")
	, m_mGameResourceList({})
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
	for (TRAVERSAL_ITER(m_mGameResourceList, it))
		Safe_Release((*it).second);

	m_mGameResourceList.clear();
}

_bool CResources::FileExists(wstring& _path)
{
	return fs::exists(_path) && fs::is_regular_file(_path);
}

_bool CResources::FileExists(string& _path)
{
	return fs::exists(_path) && fs::is_regular_file(_path);
}
