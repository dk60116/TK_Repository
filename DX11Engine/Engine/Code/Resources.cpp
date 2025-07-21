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
	LoadResourceComplete_Game(CreateGameResource<CMeshBuffer>(L"Line (Mesh Buffer)", L"Line"));
	LoadResourceComplete_Game(CreateGameResource<CMeshBuffer>(L"Rect (Mesh Buffer)", L"Rect"));
	LoadResourceComplete_Game(CreateGameResource<CMeshBuffer>(L"LineRect (Mesh Buffer)", L"LineRect"));
	LoadResourceComplete_Game(CreateGameResource<CMeshBuffer>(L"Cube (Mesh Buffer)", L"Cube"));
	LoadResourceComplete_Game(CreateGameResource<CMeshBuffer>(L"Quad (Mesh Buffer)", L"Quad"));

	CShader::SHADERDESC lineColorShaderDesc = { L"../EngineResources/Shader/DefaultLine.hlsl", L"",  VertexColorSkinnedBuffer::numElements, VertexColorSkinnedBuffer::elementDesc };
	LoadResourceComplete_Game(CreateGameResource<CShader>(L"DefaultLine (Shader)", L"", &lineColorShaderDesc));

	CShader* dlShader = LoadOnGame<CShader>(L"DefaultLine (Shader)");
	CMaterial::MATERIALDESC dlMatDesc = { dlShader, false };
	LoadResourceComplete_Game(CreateGameResource<CMaterial>(L"DefaultLineMaterial (Material)", L"", &dlMatDesc));

	CShader::SHADERDESC litShaderDesc = { L"../EngineResources/Shader/Lit.hlsl", L"", VertexSkinnedBuffer::numElements, VertexSkinnedBuffer::elementDesc };
	LoadResourceComplete_Game(CreateGameResource<CShader>(L"Lit (Shader)", L"", &litShaderDesc));

	CShader* litShader = LoadOnGame<CShader>(L"Lit (Shader)");
	CMaterial::MATERIALDESC litMatDesc = { litShader, true };
	litMatDesc.customFloatValues.push_back({ L"gSmoothness", 0.f });
	litMatDesc.customVector2Values.push_back({ L"gTiling", {1.f, 1.f} });
	litMatDesc.customVector2Values.push_back({ L"gOffset", {0.f, 0.f} });
	LoadResourceComplete_Game(CreateGameResource<CMaterial>(L"LitMaterial (Material)", L"", &litMatDesc));

	CShader::SHADERDESC unlitColorShaderDesc = { L"../EngineResources/Shader/UnlitColor.hlsl", L"",  VertexSkinnedBuffer::numElements, VertexSkinnedBuffer::elementDesc };
	LoadResourceComplete_Game(CreateGameResource<CShader>(L"UnlitColor (Shader)", L"", &unlitColorShaderDesc));

	CShader* ulcShader = LoadOnGame<CShader>(L"UnlitColor (Shader)");
	CMaterial::MATERIALDESC ulcMatDesc = { ulcShader, false };
	LoadResourceComplete_Game(CreateGameResource<CMaterial>(L"UnlitMaterial (Material)", L"", &ulcMatDesc));

	CShader::SHADERDESC outlineShaderDesc = { L"../EngineResources/Shader/Outline.hlsl", L"",  VertexSkinnedOutlineBuffer::numElements, VertexSkinnedOutlineBuffer::elementDesc };
	LoadResourceComplete_Game(CreateGameResource<CShader>(L"Outline (Shader)", L"", &outlineShaderDesc));

	CShader::SHADERDESC dUIShaderDesc = { L"../EngineResources/Shader/DefaultUI.hlsl", L"",  VertexTexColorBuffer::numElements, VertexTexColorBuffer::elementDesc };
	LoadResourceComplete_Game(CreateGameResource<CShader>(L"DefaultUI (Shader)", L"", &dUIShaderDesc));

	CShader* duiShader = LoadOnGame<CShader>(L"DefaultUI (Shader)");
	CMaterial::MATERIALDESC duiMatDesc = { duiShader, false };
	LoadResourceComplete_Game(CreateGameResource<CMaterial>(L"DefaultUIMaterial (Material)", L"", &duiMatDesc));

	return S_OK;
}

void CResources::Release()
{
	for (TRAVERSAL_ITER(m_mGameResourceList, it))
		Safe_Release((*it).second);

	m_mGameResourceList.clear();
}

void CResources::LoadResourceComplete_Game(const CEngineResource* _ptr)
{
	if (_ptr)
		CDebug::Log(L"Create Game resource successfully: " + _ptr->Get_ResourceName());
	else
		CDebug::LogError("Failed create Scene resource");
}

void CResources::LoadResourceComplete_Scene(const CEngineResource* _ptr)
{
	if (_ptr)
		CDebug::Log(L"Create Scene resource successfully: " + _ptr->Get_ResourceName());
	else
		CDebug::LogError("Failed create Scene resource");
}

vector<MeshBundle> CResources::CreateSceneMeshBundle(const wstring& _name, const wstring& _path, _int _filter, void* _desc, const _bool _tempScene)
{
	_float scaleFactor = 1.f;

	if (_desc)
		scaleFactor = *reinterpret_cast<_float*>(_desc);

	vector<MeshBundle> resultList = {};

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
	};

	for (_uint i = 0; i < scene->mNumMeshes; ++i)
	{
		MeshBundle newBundle = {};

		if (_filter & MESHBUFFER)
		{
			CMeshBuffer::MeshBufferInitiaizeInfo info = CMeshBuffer::CreateObjectMesh(scene, i, scaleFactor);
			CMeshBuffer* mb = CMeshBuffer::Create();;

			mb->Initailize_Custom(info, nullptr);
			mb->Set_ResourceName(CMeshBuffer::FindMeshName(scene, i));

			newBundle.meshBuffer = mb;
		}

		if ((_filter & MATERIAL))
		{
			if (scene->HasMaterials())
			{
				aiMaterial* newMat = scene->mMaterials[i];

				aiString texPath;
				if (newMat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == aiReturn_SUCCESS)
				{
					string path = texPath.C_Str();

					filesystem::path fbxDir = filesystem::path(_path).parent_path();
					filesystem::path texRelPath = filesystem::u8path(path);

					filesystem::path fullPath = fbxDir / texRelPath;

					wstring lastPath = m_strDefaultAssetPath + fullPath.wstring();

					CTexture* newTex = CTexture::Create();
					newTex->Initialize(lastPath, lastPath, nullptr);

					newBundle.texture = newTex;
				}
			}
		}

		if ((_filter & TEXTURE))
		{
			if (scene->HasTextures())
				aiTexture* newTex = scene->mTextures[i];
		}

		resultList.push_back(newBundle);
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

vector<MeshBundle> CResources::LoadMeshBuffersOnScene(const wstring& _name)
{
	vector<MeshBundle> r = {};

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
