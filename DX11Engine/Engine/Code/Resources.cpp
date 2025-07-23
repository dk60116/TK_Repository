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

HRESULT CResources::ConvertFBXToMeshBufferData(const wstring _filePath)
{
	Assimp::Importer importer;
	const aiScene* aiScene = importer.ReadFile
	(
		CEngineString::WStringToString(m_strDefaultAssetPath + _filePath),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_ConvertToLeftHanded |
		aiProcess_FlipUVs
	);

	if (!aiScene)
	{
		CDebug::LogError(L"Failed create mesh buffer - Can not create AIScene: " + _filePath);
		return E_FAIL;
	};

	if (!aiScene->HasMeshes())
	{
		CDebug::LogError(L"Failed create mesh buffer - AIScene has not meshes: " + _filePath);
		return E_FAIL;
	}

	using VTX = VertexTexNormalTangentBuffer;

	vector<CMeshBuffer::MeshBufferInitiaizeInfo> bufferInfoList = {};

	if (!aiScene)
	{
		OutputDebugStringA("Assimp load failed or mesh index out of bounds.\n");
		return E_FAIL;
	}

	const _bool hasMaterial = aiScene->HasMaterials();

	for (_uint i = 0; i < aiScene->mNumMeshes; ++i)
	{
		CMeshBuffer::MeshBufferInitiaizeInfo info = {};

		const aiMesh* mesh = aiScene->mMeshes[i];

		info.meshName = CMeshBuffer::FindMeshName(aiScene, i);

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

		if (hasMaterial)
		{
			aiMaterial* newMat = aiScene->mMaterials[i];

			aiString texPath;
			if (newMat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == aiReturn_SUCCESS)
			{
				string path = texPath.C_Str();

				filesystem::path fbxDir = filesystem::path(_filePath).parent_path();
				filesystem::path texRelPath = filesystem::u8path(path);

				filesystem::path fullPath = fbxDir / texRelPath;

				wstring lastPath = m_strDefaultAssetPath + fullPath.wstring();

				info.diffuseMapPath = lastPath;
			}
		}

		bufferInfoList.push_back(info);
	}

	auto splitPath = CEngineString::Split(_filePath, L"/");

	wstring fileFolder = splitPath[splitPath.size() - 2];
	wstring fileNameExt = splitPath[splitPath.size() - 1];
	
	auto pureName = CEngineString::Split(fileNameExt, L".")[0];

	wstring saveName = fileFolder + L"_" + pureName;


	if (FAILED(SaveMeshBufferInfos(L"BinaryAssets/" + saveName + L".meshdata", bufferInfoList)))
	{
		CDebug::LogError(L"Failed ceate mesh Data - can not save: " + _filePath);
		return E_FAIL;
	}

	CDebug::Log(L"Complete ceate mesh Data: " + _filePath);

	return S_OK;
}

HRESULT CResources::ConvertFBXToSkinnedBufferData(const wstring _filePath)
{
	Assimp::Importer importer;
	const aiScene* aiScene = importer.ReadFile
	(
		CEngineString::WStringToString(m_strDefaultAssetPath + _filePath),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_ConvertToLeftHanded |
		aiProcess_FlipUVs
	);

	if (!aiScene)
	{
		CDebug::LogError(L"Failed create skinned buffer - Can not create AIScene: " + _filePath);
		return E_FAIL;
	}

	if (!aiScene->HasMeshes())
	{
		CDebug::LogError(L"Failed create skinned buffer - AIScene has not meshes: " + _filePath);
		return E_FAIL;
	}

	_bool hasBones = false;

	for (_uint i = 0; i < aiScene->mNumMeshes; ++i)
	{
		if (aiScene->mMeshes[i]->HasBones())
			hasBones = true;
	}

	if (!hasBones)
	{
		CDebug::LogError(L"Failed create skinned buffer - AIMesh has not bones: " + _filePath);
		return E_FAIL;
	}

	const _bool hasMaterial = aiScene->HasMaterials();

	using VTX = VertexSkinnedBuffer;

	vector<CSkinnedMeshBuffer::SkinnedBufferInitiaizeInfo> bufferInfoList = {};
	
	for (_uint i = 0; i < aiScene->mNumMeshes; ++i)
	{
		CSkinnedMeshBuffer::SkinnedBufferInitiaizeInfo info = {};

		const aiMesh* mesh = aiScene->mMeshes[i];

		info.meshName = CMeshBuffer::FindMeshName(aiScene, i);

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

		unordered_map<string, _uint> boneNameToIndex;
		_uint boneIndexCounter = 0;

		for (_uint b = 0; b < mesh->mNumBones; ++b)
		{
			aiBone* bone = mesh->mBones[b];
			string boneName = bone->mName.C_Str();

			if (boneNameToIndex.find(boneName) == boneNameToIndex.end())
				boneNameToIndex[boneName] = boneIndexCounter++;

			_uint boneIndex = boneNameToIndex[boneName];

			for (_uint w = 0; w < bone->mNumWeights; ++w)
			{
				const aiVertexWeight& weight = bone->mWeights[w];

				_uint vertexId = weight.mVertexId;
				_float boneWeight = weight.mWeight;

				if (vertexId < vertices.size())
					CSkinnedMeshBuffer::FillBoneWeights(vertices[vertexId], boneIndex, boneWeight);
			}
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

		if (hasMaterial)
		{
			aiMaterial* newMat = aiScene->mMaterials[i];

			aiString texPath;
			if (newMat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == aiReturn_SUCCESS)
			{
				string path = texPath.C_Str();

				filesystem::path fbxDir = filesystem::path(_filePath).parent_path();
				filesystem::path texRelPath = filesystem::u8path(path);

				filesystem::path fullPath = fbxDir / texRelPath;

				wstring lastPath = m_strDefaultAssetPath + fullPath.wstring();

				info.diffuseMapPath = lastPath;
			}
		}

		for (_uint i = 0; i < mesh->mNumBones; ++i)
		{
			string boneName = mesh->mBones[i]->mName.C_Str();
			info.boneNames.push_back(CEngineString::StringToWString(boneName));

			auto aiMat = mesh->mBones[i]->mOffsetMatrix;

			_float4x4 float44 = XMFLOAT4X4
			(
				aiMat.a1, aiMat.a2, aiMat.a3, aiMat.a4,
				aiMat.b1, aiMat.b2, aiMat.b3, aiMat.b4,
				aiMat.c1, aiMat.c2, aiMat.c3, aiMat.c4,
				aiMat.d1, aiMat.d2, aiMat.d3, aiMat.d4
			);

			info.boneOffsetMatrices.push_back(float44);
		}

		bufferInfoList.push_back(info);
	}

	auto splitPath = CEngineString::Split(_filePath, L"/");

	wstring fileFolder = splitPath[splitPath.size() - 2];
	wstring fileNameExt = splitPath[splitPath.size() - 1];

	auto pureName = CEngineString::Split(fileNameExt, L".")[0];

	wstring saveName = fileFolder + L"_" + pureName;

	if (FAILED(SaveSkinnedBufferInfos(L"BinaryAssets/" + saveName + L".skinneddata", bufferInfoList)))
	{
		CDebug::LogError(L"Failed ceate skinned mesh Data - can not save: " + _filePath);
		return E_FAIL;
	}

	CDebug::Log(L"Complete ceate skinned mesh Data: " + _filePath);

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
		_uint nameSize = static_cast<_uint>(info.meshName.size());
		out.write(reinterpret_cast<char*>(&nameSize), sizeof(_uint));
		if (nameSize > 0)
			out.write(reinterpret_cast<const char*>(info.meshName.data()), sizeof(wchar_t) * nameSize);

		_uint bufferSize = static_cast<_uint>(info.buffer.size());
		out.write(reinterpret_cast<char*>(&bufferSize), sizeof(_uint));
		if (bufferSize > 0)
			out.write(reinterpret_cast<const char*>(info.buffer.data()), bufferSize);

		_uint indicesSize = static_cast<_uint>(info.indices.size());
		out.write(reinterpret_cast<char*>(&indicesSize), sizeof(_uint));
		if (indicesSize > 0)
			out.write(reinterpret_cast<const char*>(info.indices.data()), sizeof(_uint) * indicesSize);

		out.write(reinterpret_cast<const char*>(&info.desc), sizeof(CMeshBuffer::MESHBUFFERDESC));

		_uint diffuseTexPathSize = static_cast<_uint>(info.diffuseMapPath.size());
		out.write(reinterpret_cast<char*>(&diffuseTexPathSize), sizeof(_uint));
		if (diffuseTexPathSize > 0)
			out.write(reinterpret_cast<const char*>(info.diffuseMapPath.data()), sizeof(wchar_t) * diffuseTexPathSize);
	}

	out.close();

	CDebug::Log(L"Save complete meshdata: " + _filePath);

	return S_OK;
}

vector<CMeshBuffer::MeshBufferInitiaizeInfo> CResources::ReadMeshBufferInfos(const wstring _binFileName)
{
	using namespace std;

	vector<CMeshBuffer::MeshBufferInitiaizeInfo> infoList = {};

	ifstream in(L"BinaryAssets/" + _binFileName, ios::binary);

	if (!in.is_open())
		return {};

	_uint count = 0;
	in.read(reinterpret_cast<char*>(&count), sizeof(_uint));

	for (_uint i = 0; i < count; ++i)
	{
		CMeshBuffer::MeshBufferInitiaizeInfo info = {};

		_uint nameCount = 0;
		in.read(reinterpret_cast<char*>(&nameCount), sizeof(_uint));
		if (nameCount > 0)
		{
			info.meshName.resize(nameCount);
			in.read(reinterpret_cast<char*>(info.meshName.data()), sizeof(wchar_t) * nameCount);
		}

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

		_uint diffuseTexPathCount = 0;
		in.read(reinterpret_cast<char*>(&diffuseTexPathCount), sizeof(_uint));
		if (indexCount > 0)
		{
			info.diffuseMapPath.resize(diffuseTexPathCount);
			in.read(reinterpret_cast<char*>(info.diffuseMapPath.data()), sizeof(wchar_t) * diffuseTexPathCount);
		}

		infoList.push_back(info);
	}

	in.close();

	return infoList;
}

HRESULT CResources::SaveSkinnedBufferInfos(const wstring _filePath, vector<CSkinnedMeshBuffer::SkinnedBufferInitiaizeInfo> _infoList)
{
	using namespace std;

	ofstream out(_filePath, ios::binary);

	if (!out.is_open())
		return E_FAIL;

	_uint count = static_cast<_uint>(_infoList.size());
	out.write(reinterpret_cast<const char*>(&count), sizeof(_uint));

	for (const auto& info : _infoList)
	{
		_uint nameSize = static_cast<_uint>(info.meshName.size());
		out.write(reinterpret_cast<char*>(&nameSize), sizeof(_uint));
		if (nameSize > 0)
			out.write(reinterpret_cast<const char*>(info.meshName.data()), sizeof(wchar_t) * nameSize);

		_uint bufferSize = static_cast<_uint>(info.buffer.size());
		out.write(reinterpret_cast<char*>(&bufferSize), sizeof(_uint));
		if (bufferSize > 0)
			out.write(reinterpret_cast<const char*>(info.buffer.data()), bufferSize);

		_uint indicesSize = static_cast<_uint>(info.indices.size());
		out.write(reinterpret_cast<char*>(&indicesSize), sizeof(_uint));
		if (indicesSize > 0)
			out.write(reinterpret_cast<const char*>(info.indices.data()), sizeof(_uint) * indicesSize);

		out.write(reinterpret_cast<const char*>(&info.desc), sizeof(CMeshBuffer::MESHBUFFERDESC));

		_uint diffuseTexPathSize = static_cast<_uint>(info.diffuseMapPath.size());
		out.write(reinterpret_cast<char*>(&diffuseTexPathSize), sizeof(_uint));
		if (diffuseTexPathSize > 0)
			out.write(reinterpret_cast<const char*>(info.diffuseMapPath.data()), sizeof(wchar_t) * diffuseTexPathSize);

		_uint boneNamesSize = static_cast<_uint>(info.boneNames.size());
		out.write(reinterpret_cast<char*>(&boneNamesSize), sizeof(_uint));
		if (boneNamesSize > 0)
		{
			for (_uint i = 0; i < boneNamesSize; ++i)
			{
				_uint size = static_cast<_uint>(info.boneNames[i].size());
				out.write(reinterpret_cast<char*>(&size), sizeof(_uint));
				out.write(reinterpret_cast<const char*>(info.boneNames[i].data()), sizeof(wchar_t) * size);
			}
		}

		_uint boneMatricesSize = static_cast<_uint>(info.boneOffsetMatrices.size());
		out.write(reinterpret_cast<char*>(&boneMatricesSize), sizeof(_uint));
		if (boneMatricesSize > 0)
			out.write(reinterpret_cast<const char*>(info.boneOffsetMatrices.data()), sizeof(_float4x4) * boneMatricesSize);
	}

	out.close();

	CDebug::Log(L"Save complete skinneddata: " + _filePath);

	return S_OK;
}

vector<CSkinnedMeshBuffer::SkinnedBufferInitiaizeInfo> CResources::ReadSkinnedBufferInfos(const wstring _binFileName)
{
	using namespace std;

	vector<CSkinnedMeshBuffer::SkinnedBufferInitiaizeInfo> infoList = {};

	ifstream in(L"BinaryAssets/" + _binFileName, ios::binary);

	if (!in.is_open())
		return {};

	_uint count = 0;
	in.read(reinterpret_cast<char*>(&count), sizeof(_uint));

	for (_uint i = 0; i < count; ++i)
	{
		CSkinnedMeshBuffer::SkinnedBufferInitiaizeInfo info = {};

		_uint nameCount = 0;
		in.read(reinterpret_cast<char*>(&nameCount), sizeof(_uint));
		if (nameCount > 0)
		{
			info.meshName.resize(nameCount);
			in.read(reinterpret_cast<char*>(info.meshName.data()), sizeof(wchar_t) * nameCount);
		}

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

		_uint diffuseTexPathCount = 0;
		in.read(reinterpret_cast<char*>(&diffuseTexPathCount), sizeof(_uint));
		if (indexCount > 0)
		{
			info.diffuseMapPath.resize(diffuseTexPathCount);
			in.read(reinterpret_cast<char*>(info.diffuseMapPath.data()), sizeof(wchar_t) * diffuseTexPathCount);
		}

		_uint boneNamesCount = 0;
		in.read(reinterpret_cast<char*>(&boneNamesCount), sizeof(_uint));
		if (boneNamesCount > 0)
		{
			for (_uint i = 0; i < boneNamesCount; ++i)
			{
				_uint size = 0;
				in.read(reinterpret_cast<char*>(&size), sizeof(_uint));

				wstring name;
				if (size > 0)
				{
					name.resize(size);
					in.read(reinterpret_cast<char*>(name.data()), sizeof(wchar_t) * size);
				}

				info.boneNames.push_back(name);
			}
		}

		_uint boneMatrixCount = 0;
		in.read(reinterpret_cast<char*>(&boneMatrixCount), sizeof(_uint));
		if (boneMatrixCount > 0)
		{
			for (_uint i = 0; i < boneMatrixCount; ++i)
			{
				_float4x4 matrix = {};
				in.read(reinterpret_cast<char*>(&matrix), sizeof(_float4x4));
				info.boneOffsetMatrices.push_back(matrix);
			}
		}

		infoList.push_back(info);
	}

	in.close();

	return infoList;
}

vector<MeshBundle> CResources::CreateSceneMeshBundle(const wstring& _name, vector<CMeshBuffer::MeshBufferInitiaizeInfo> _infoList, _int _filter, void* _desc, const _bool _tempScene)
{
	if (_infoList.size() <= 0)
	{
		CDebug::LogError(L"Failed create SceneMeshBundle - Empty list: " + _name);
		return {};
	}

	_float scaleFactor = 1.f;

	if (_desc)
		scaleFactor = *reinterpret_cast<_float*>(_desc);

	vector<MeshBundle> resultList = {};

	for (_uint i = 0; i < _infoList.size(); ++i)
	{
		MeshBundle newBundle;

		if (_filter & FILTER_MESHBUFFER)
		{
			CMeshBuffer* newBuffer = CMeshBuffer::Create();
			newBuffer->Initailize_Custom(_infoList[i], _desc);

			newBundle.meshBuffer = newBuffer;
		}

		if (_filter & FILTER_MATERIAL)
		{
			CTexture* newTex = CTexture::Create();
			newTex->Initialize(_infoList[i].diffuseMapPath, _infoList[i].diffuseMapPath, nullptr);

			newBundle.texture = newTex;
		}

		resultList.push_back(newBundle);
	}

	CScene* targetScene = _tempScene ? CSceneManager::GetInstance().Get_TempScene() :
		CSceneManager::GetInstance().Get_CrtScene();

	if (!_tempScene)
		targetScene->Add_MeshBundle(_name, resultList);
	else
		targetScene->Add_TempMeshBundle(_name, resultList);

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
