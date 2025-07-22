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

HRESULT CResources::ConvertFBXToMeshBufferData(const wstring _readFilePath)
{
	Assimp::Importer importer;
	const aiScene* aiScene = importer.ReadFile
	(
		CEngineString::WStringToString(m_strDefaultAssetPath + _readFilePath),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_ConvertToLeftHanded |
		aiProcess_FlipUVs
	);

	if (!aiScene)
	{
		CDebug::LogError(L"Create Scene mesh bundle failed - Can not create AIScene: " + _readFilePath);
		return E_FAIL;
	};

	if (!aiScene->HasMeshes())
	{
		CDebug::LogError(L"Create Scene mesh bundle failed - AIScene not has meshes: " + _readFilePath);
		return E_FAIL;
	}

	using VTX = VertexTexNormalTangentBuffer;

	vector<CMeshBuffer::MeshBufferInitiaizeInfo> bufferList = {};

	if (!aiScene)
	{
		OutputDebugStringA("Assimp load failed or mesh index out of bounds.\n");
		return E_FAIL;
	}

	for (size_t i = 0; i < aiScene->mNumMeshes; ++i)
	{
		CMeshBuffer::MeshBufferInitiaizeInfo info = {};

		const aiMesh* mesh = aiScene->mMeshes[i];

		vector<VTX> vertices;
		vector<_uint> indices;

		// 정점 복사
		for (_uint j = 0; j < mesh->mNumVertices; ++j)
		{
			VTX v{};
			v.position = 
			{
				mesh->mVertices[j].x,
				mesh->mVertices[j].y,
				mesh->mVertices[j].z
			};

			v.normal = mesh->HasNormals() ?
				_float3{ mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z } :
				_float3{ 0, 0, 0 };

			v.uv = mesh->HasTextureCoords(0) ?
				_float2{ mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y } :
				_float2{ 0, 0 };

			v.tangent = mesh->HasTangentsAndBitangents() ?
				_float3{ mesh->mTangents[j].x, mesh->mTangents[j].y, mesh->mTangents[j].z } :
				_float3{ 0, 0, 0 };

			vertices.emplace_back(v);
		}

		// 인덱스 복사
		for (_uint f = 0; f < mesh->mNumFaces; ++f)
		{
			const aiFace& face = mesh->mFaces[f];
			if (face.mNumIndices != 3) continue;
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		// 버퍼 정보 세팅
		CMeshBuffer::MESHBUFFERDESC desc{};
		desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		desc.vertexSize = sizeof(VTX);
		desc.vertextCount = static_cast<_uint>(vertices.size());
		desc.indexCount = static_cast<_uint>(indices.size());

		info.buffer.assign
		(
			reinterpret_cast<const uint8_t*>(vertices.data()),
			reinterpret_cast<const uint8_t*>(vertices.data()) + sizeof(VTX) * vertices.size()
		);

		info.indices.assign(indices.begin(), indices.end());
		info.desc = desc;

		bufferList.push_back(info);
	}

	auto splitPath = CEngineString::Split(_readFilePath, L"/");

	wstring fileFolder = splitPath[splitPath.size() - 2];
	wstring fileNameExt = splitPath[splitPath.size() - 1];
	
	auto pureName = CEngineString::Split(fileNameExt, L".")[0];

	wstring saveName = fileFolder + L"_" + pureName;

	if (FAILED(SaveMeshBufferInfos(L"BinaryAssets/" + saveName + L".meshdata", bufferList)))
		return E_FAIL;

	return S_OK;
}

HRESULT CResources::ConverFBXToMaterialTextureData(const wstring _readFilePath)
{
	return S_OK;
}

HRESULT CResources::SaveMeshBufferInfos(const wstring _filePath, vector<CMeshBuffer::MeshBufferInitiaizeInfo> _infoList)
{
	using namespace std;

	ofstream out(_filePath, ios::binary);

	if (!out.is_open())
		return E_FAIL;

	_uint count = static_cast<_uint>(_infoList.size());
	out.write(reinterpret_cast<const char*>(&count), sizeof(_uint));

	for (const auto& info : _infoList)
	{
		_uint bufferSize = static_cast<_uint>(info.buffer.size());
		out.write(reinterpret_cast<char*>(&bufferSize), sizeof(_uint));
		if (bufferSize > 0)
			out.write(reinterpret_cast<const char*>(info.buffer.data()), bufferSize);

		_uint indicesSize = static_cast<_uint>(info.indices.size());
		out.write(reinterpret_cast<char*>(&indicesSize), sizeof(_uint));
		if (indicesSize > 0)
			out.write(reinterpret_cast<const char*>(info.indices.data()), sizeof(_uint) * indicesSize);

		out.write(reinterpret_cast<const char*>(&info.desc), sizeof(CMeshBuffer::MESHBUFFERDESC));
	}

	out.close();

	CDebug::Log(L"Save complete meshdata: " + _filePath);

	return S_OK;
}

vector<CMeshBuffer::MeshBufferInitiaizeInfo> CResources::ReadMeshBufferInfos(const wstring _binFileName)
{
	vector<CMeshBuffer::MeshBufferInitiaizeInfo> infoList = {};

	using namespace std;

	ifstream in(L"BinaryAssets/" + _binFileName, ios::binary);

	if (!in.is_open())
		return {};

	_uint count = 0;
	in.read(reinterpret_cast<char*>(&count), sizeof(_uint));

	for (_uint i = 0; i < count; ++i)
	{
		CMeshBuffer::MeshBufferInitiaizeInfo info = {};

		_uint bufferSize = 0;
		in.read(reinterpret_cast<char*>(&bufferSize), sizeof(_uint));
		if (bufferSize > 0)
		{
			info.buffer.resize(bufferSize);
			in.read(reinterpret_cast<char*>(info.buffer.data()), bufferSize);
		}

		_uint indexCount = 0;
		in.read(reinterpret_cast<char*>(&indexCount), sizeof(_uint));
		if (indexCount > 0)
		{
			info.indices.resize(indexCount);
			in.read(reinterpret_cast<char*>(info.indices.data()), sizeof(_uint) * indexCount);
		}

		in.read(reinterpret_cast<char*>(&info.desc), sizeof(CMeshBuffer::MESHBUFFERDESC));

		infoList.push_back(info);
	}

	in.close();

	return infoList;
}

vector<MeshBundle> CResources::CreateSceneMeshBundle(const wstring& _name, const wstring& _path, _int _filter, void* _desc, const _bool _tempScene)
{
	_float scaleFactor = 1.f;

	if (_desc)
		scaleFactor = *reinterpret_cast<_float*>(_desc);

	vector<MeshBundle> resultList = {};

	for (_uint i = 0; i < scene->mNumMeshes; ++i)
	{
		MeshBundle newBundle = {};

		if (_filter & MESHBUFFER)
		{
			CMeshBuffer::MeshBufferInitiaizeInfo info = CMeshBuffer::CreateObjectMesh(scene, i, scaleFactor);
			CMeshBuffer* mb = CMeshBuffer::Create();

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
