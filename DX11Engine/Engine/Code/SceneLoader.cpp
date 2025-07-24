#include "epch.h"
#include "SceneLoader.h"

CSceneLoader::CSceneLoader()
	: m_hThread(nullptr)
	, m_pCriticalSection()
	, m_mReadyFiles_Name({})
	, m_mReadyFiles_Path({})
	, m_mReadyFiles_Format({})
	, m_bRunning(false)
	, m_bLoading(true)
{
}

CSceneLoader::~CSceneLoader()
{
	Shutdown();
}

CSceneLoader& CSceneLoader::GetInstance()
{
	static CSceneLoader inst;
	return inst;
}

unsigned __stdcall CSceneLoader::ThreadMain(void* pParam)
{
	auto loader = static_cast<CSceneLoader*>(pParam);
	loader->ThreadLoadingLoop();
	return 0;
}

HRESULT CSceneLoader::Initialize()
{
	InitializeCriticalSection(&m_pCriticalSection);

	m_hThread = (HANDLE)_beginthreadex(
		nullptr, 0, ThreadMain, this, 0, nullptr);

	if (!m_hThread)
		return E_FAIL;

	m_bRunning = true;

	return S_OK;
}

const _bool CSceneLoader::Is_Loading() const
{
	return m_bLoading;
}

void CSceneLoader::StartLoading(vector<string>& _nameList, vector<string>& _fileList, vector<string>& _formatList)
{
	EnterCriticalSection(&m_pCriticalSection);

	m_mReadyFiles_Name = _nameList;
	m_mReadyFiles_Path = _fileList;
	m_mReadyFiles_Format = _formatList;

	m_bLoading = true;

	LeaveCriticalSection(&m_pCriticalSection);
}

void CSceneLoader::ThreadLoadingLoop()
{
	while (m_bRunning)
	{
		EnterCriticalSection(&m_pCriticalSection);

		if (!m_mReadyFiles_Name.empty())
		{
			m_bLoading = true;

			string name = m_mReadyFiles_Name.back();
			string file = m_mReadyFiles_Path.back();
			string format = m_mReadyFiles_Format.back();

			m_mReadyFiles_Name.pop_back();
			m_mReadyFiles_Path.pop_back();
			m_mReadyFiles_Format.pop_back();

			LeaveCriticalSection(&m_pCriticalSection);

			wstring wName = CEngineString::StringToWString(name);
			wstring wFile = CEngineString::StringToWString(file);
			wstring wFormat = CEngineString::StringToWString(format);

			if (CEngineString::Contains(wFile, L".png"))
			{
				if (CEngineString::Contains(wFormat, L"[Texture]"))
					CResources::LoadResourceComplete_Scene(CResources::GetInstance().CreateSceneResource<CTexture>(wName + L" (Texture)", wFile, nullptr, true));
			}
			else if (CEngineString::Contains(wFile, L".fbx"))
			{
				if (CEngineString::Contains(wFormat, L"[Mesh]"))
				{
					_int filter = FILTER_MESHBUFFER;

					if (CEngineString::Contains(wFormat, L"[Material]"))
						filter |= FILTER_MATERIAL;
					if (CEngineString::Contains(wFormat, L"[Texture]"))
						filter |= FILTER_TEXTURE;
					if (CEngineString::Contains(wFormat, L"[Bone]"))
						filter |= FILTER_BONE;

					auto meshDataSplit = CEngineString::Split(wFile, L"/");
					wstring meshDataFolder = meshDataSplit[meshDataSplit.size() - 2];
					wstring meshDataTail = meshDataSplit[meshDataSplit.size() - 1];
					wstring meshDataName = CEngineString::Split(meshDataTail, L".")[0];

					const wstring meshdataPath = meshDataFolder + L"_" + meshDataName + L".meshdata";

					auto meshInfoList = CResources::GetInstance().ReadMeshBufferInfos(meshdataPath);

					CResources::GetInstance().CreateSceneMeshBundle(wName + L" (MeshBuffer)", meshInfoList, filter, nullptr, true);
				}
				if (CEngineString::Contains(wFormat, L"[Skinned Mesh]"))
				{
					_int filter = FILTER_MESHBUFFER;

					if (CEngineString::Contains(wFormat, L"[Bone]"))
						filter |= FILTER_BONE;

					auto skinnedDataSplit = CEngineString::Split(wFile, L"/");
					wstring skinnedDataFolder = skinnedDataSplit[skinnedDataSplit.size() - 2];
					wstring skinnedDataTail = skinnedDataSplit[skinnedDataSplit.size() - 1];
					wstring skinnedDataName = CEngineString::Split(skinnedDataTail, L".")[0];

					const wstring skinneddataPath = skinnedDataFolder + L"_" + skinnedDataName + L".skinneddata";

					auto skinnedInfoList = CResources::GetInstance().ReadSkinnedBufferInfos(skinneddataPath);

					CResources::GetInstance().CreateSceneSkinnedBundle(wName + L" (MeshBuffer)", skinnedInfoList.initList, skinnedInfoList.skeletalList, filter, nullptr, true);
				}
				if (CEngineString::Contains(wFormat, L"[Animation Clip]"))
					CResources::LoadResourceComplete_Scene(CResources::GetInstance().CreateSceneResource<CAnimationClip>(wName + L" (Animation)", wFile, nullptr, true));
			}
			else if (wFile == L"Terrain")
			{
				CMeshBuffer::TERRAINBUFFERDESC terranDesc = FormatToTerrainDesc(wName, wFormat);
				CResources::LoadResourceComplete_Scene(CResources::GetInstance().CreateSceneResource<CMeshBuffer>(wName + L" (Terrain MeshBuffer)", wFile, &terranDesc, true));
			}
		}
		else
		{
			m_bLoading = false;
			LeaveCriticalSection(&m_pCriticalSection);
			Sleep(10);
		}
	}
}

void CSceneLoader::Shutdown()
{
	m_bRunning = false;

	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_pCriticalSection);
}

CMeshBuffer::TERRAINBUFFERDESC CSceneLoader::FormatToTerrainDesc(wstring _name, wstring _format) const
{
	CMeshBuffer::TERRAINBUFFERDESC terrainDesc = {};

	wstring terrainFormat = CEngineString::Erase(_format, L"[");
	terrainFormat = CEngineString::Erase(terrainFormat, L"]");

	vector<wstring> tokens = CEngineString::Split(terrainFormat, L", ");
	vector<_float> values = {};

	for (size_t i = 0; i < 5; ++i)
		values.push_back(stof(tokens[i]));

	terrainDesc.isHeightMapBase = values[0] > 0.5f;
	terrainDesc.landscape = static_cast<_uint>(values[1]);
	terrainDesc.portrait = static_cast<_uint>(values[2]);
	terrainDesc.size = values[3];
	terrainDesc.heightWeight = values[4];
	CResources::LoadResourceComplete_Scene(CResources::GetInstance().CreateSceneResource<CTexture>(_name + L" - Terrain Height map (Texture)", tokens[5], nullptr, true));
	terrainDesc.heightMap = _name + L" - Terrain Height map (Texture)";

	return terrainDesc;
}
