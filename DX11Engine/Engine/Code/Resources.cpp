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

HRESULT CResources::Initialize()
{
	LoadComplete_Game(CreateGameResource<CMeshBuffer>(L"Cube", L"Cube"));

	CShader::SHADERDESC unlitColorShaderDesc = { L"../EngineResource/Shader/UnlitColor.hlsl", L"",  VertexSkinnedBuffer::numElements, VertexSkinnedBuffer::elemetDesc };
	LoadComplete_Game(CreateGameResource<CShader>(L"UnlitColor (Shader)", L"", &unlitColorShaderDesc));

	CShader::SHADERDESC outlineShaderDesc = { L"../EngineResource/Shader/Outline.hlsl", L"",  VertexSkinnedOutlineBuffer::numElements, VertexSkinnedOutlineBuffer::elemetDesc };
	LoadComplete_Game(CreateGameResource<CShader>(L"Outline (Shader)", L"", &outlineShaderDesc));

	return S_OK;
}

void CResources::Release()
{
	for (TRAVERSAL_ITER(m_mGameResourceList, it))
		Safe_Release((*it).second);

	m_mGameResourceList.clear();
}

void CResources::LoadComplete_Game(const CEngineResource* _ptr)
{
	if (_ptr)
		CDebug::Log(L"Create Game resource successed: " + _ptr->Get_ResourceName());
	else
		CDebug::LogError("Create Game resource failed");
}

void CResources::LoadComplete_Scene(const CEngineResource* _ptr)
{
	if (_ptr)
		CDebug::Log(L"Create Scene resource successed: " + _ptr->Get_ResourceName());
	else
		CDebug::LogError("Create Scene resource failed");
}

_bool CResources::FileExists(wstring& _path)
{
	return fs::exists(_path) && fs::is_regular_file(_path);
}

_bool CResources::FileExists(string& _path)
{
	return fs::exists(_path) && fs::is_regular_file(_path);
}
