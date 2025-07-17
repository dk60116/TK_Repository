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
	LoadComplete_Game(CreateGameResource<CMeshBuffer>(L"Line (Mesh Buffer)", L"Line"));
	LoadComplete_Game(CreateGameResource<CMeshBuffer>(L"Rect (Mesh Buffer)", L"Rect"));
	LoadComplete_Game(CreateGameResource<CMeshBuffer>(L"LineRect (Mesh Buffer)", L"LineRect"));
	LoadComplete_Game(CreateGameResource<CMeshBuffer>(L"Cube (Mesh Buffer)", L"Cube"));
	LoadComplete_Game(CreateGameResource<CMeshBuffer>(L"Quad (Mesh Buffer)", L"Quad"));

	CShader::SHADERDESC lineColorShaderDesc = { L"../EngineResources/Shader/DefaultLine.hlsl", L"",  VertexColorSkinnedBuffer::numElements, VertexColorSkinnedBuffer::elementDesc };
	LoadComplete_Game(CreateGameResource<CShader>(L"DefaultLine (Shader)", L"", &lineColorShaderDesc));

	CShader* dlShader = LoadOnGame<CShader>(L"DefaultLine (Shader)");
	CMaterial::MATERIALDESC dlMatDesc = { dlShader, false };
	LoadComplete_Game(CreateGameResource<CMaterial>(L"DefaultLineMaterial (Material)", L"", &dlMatDesc));

	CShader::SHADERDESC litShaderDesc = { L"../EngineResources/Shader/Lit.hlsl", L"", VertexSkinnedBuffer::numElements, VertexSkinnedBuffer::elementDesc };
	LoadComplete_Game(CreateGameResource<CShader>(L"Lit (Shader)", L"", &litShaderDesc));

	CShader* litShader = LoadOnGame<CShader>(L"Lit (Shader)");
	CMaterial::MATERIALDESC litMatDesc = { litShader, true };
	LoadComplete_Game(CreateGameResource<CMaterial>(L"LitMaterial (Material)", L"", &litMatDesc));

	CShader::SHADERDESC unlitColorShaderDesc = { L"../EngineResources/Shader/UnlitColor.hlsl", L"",  VertexSkinnedBuffer::numElements, VertexSkinnedBuffer::elementDesc };
	LoadComplete_Game(CreateGameResource<CShader>(L"UnlitColor (Shader)", L"", &unlitColorShaderDesc));

	CShader* ulcShader = LoadOnGame<CShader>(L"UnlitColor (Shader)");
	CMaterial::MATERIALDESC ulcMatDesc = { ulcShader, false };
	LoadComplete_Game(CreateGameResource<CMaterial>(L"UnlitMaterial (Material)", L"", &ulcMatDesc));

	CShader::SHADERDESC outlineShaderDesc = { L"../EngineResources/Shader/Outline.hlsl", L"",  VertexSkinnedOutlineBuffer::numElements, VertexSkinnedOutlineBuffer::elementDesc };
	LoadComplete_Game(CreateGameResource<CShader>(L"Outline (Shader)", L"", &outlineShaderDesc));

	CShader::SHADERDESC dUIShaderDesc = { L"../EngineResources/Shader/DefaultUI.hlsl", L"",  VertexTexColorBuffer::numElements, VertexTexColorBuffer::elementDesc };
	LoadComplete_Game(CreateGameResource<CShader>(L"DefaultUI (Shader)", L"", &dUIShaderDesc));

	CShader* duiShader = LoadOnGame<CShader>(L"DefaultUI (Shader)");
	CMaterial::MATERIALDESC duiMatDesc = { duiShader, false };
	LoadComplete_Game(CreateGameResource<CMaterial>(L"DefaultUIMaterial (Material)", L"", &duiMatDesc));

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
		CDebug::Log(L"Create Game resource successfully: " + _ptr->Get_ResourceName());
	else
		CDebug::LogError("Create Game resource failed");
}

void CResources::LoadComplete_Scene(const CEngineResource* _ptr)
{
	if (_ptr)
		CDebug::Log(L"Create Scene resource successfully: " + _ptr->Get_ResourceName());
	else
		CDebug::LogError("Create Scene resource failed");
}

vector<CMeshBuffer*> CResources::CreateSceneMeshBuffers(const wstring& _name, const wstring& _path, void* _desc, const _bool _tempScene)
{
	_float scaleFactor = 1.f;

	if (_desc)
		scaleFactor = *reinterpret_cast<_float*>(_desc);

	vector<CMeshBuffer*> resultList = {};

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile
	(
		CEngineString::WStringToString(m_strDefaultAssetPath + _path),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_ConvertToLeftHanded |
		aiProcess_FlipUVs
	);

	if (!scene)
	{
		CDebug::LogError(L"Create Scene mesh bundle failed: " + _path);
		return {};
	}

	for (_uint i = 0; i < scene->mNumMeshes; ++i)
	{
		CMeshBuffer::MeshBufferInitiaizeInfo info = CMeshBuffer::CreateObjectMesh(scene, i, scaleFactor);
		CMeshBuffer* mb = CMeshBuffer::Create();;

		mb->Initailize_Custom(info, nullptr);
		mb->Set_ResourceName(CMeshBuffer::FindMeshName(scene, i));

		resultList.push_back(mb);
	}

	CScene* targetScene = _tempScene ? CSceneManager::GetInstance().Get_TempScene() :
		CSceneManager::GetInstance().Get_CrtScene();

	if (!_tempScene)
		targetScene->Add_MeshBundle(_name, resultList);
	else
		targetScene->Add_TempMeshBundle(_name, resultList);

	CDebug::Log(L"Create Scene Scene mesh bundle successfully: " + _name);

	return resultList;
}

vector<CMeshBuffer*> CResources::LoadMeshBuffersOnScene(const wstring& _name)
{
	vector<CMeshBuffer*> r = {};

	if (CSceneManager::GetInstance().Get_CrtScene())
		r = CSceneManager::GetInstance().Get_CrtScene()->Find_MeshInfoResource(_name);

	return r;
}

_bool CResources::FileExists(const wstring& _path)
{
	const string path = CEngineString::WStringToString(_path);
	return FileExists(path);
}

_bool CResources::FileExists(const string& _path)
{
	DWORD attrib = GetFileAttributesA(_path.c_str());
	return (attrib != INVALID_FILE_ATTRIBUTES) && !(attrib & FILE_ATTRIBUTE_DIRECTORY);
}
