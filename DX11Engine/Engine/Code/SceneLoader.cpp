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
	, m_iTootalFile(0)
	, m_iLoadedFile(0)
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
	InitializeCriticalSection(&GetInstance().m_pCriticalSection);

	GetInstance().m_hThread = (HANDLE)_beginthreadex
	( nullptr, 0, ThreadMain, &GetInstance(), 0, nullptr);

	if (!GetInstance().m_hThread)
		return E_FAIL;

	GetInstance().m_bRunning = true;

	return S_OK;
}

const _bool CSceneLoader::Is_Loading()
{
	return GetInstance().m_bLoading;
}

const _float CSceneLoader::Get_LoadingProgress()
{
	return static_cast<float>(GetInstance().m_iLoadedFile) / GetInstance().m_iTootalFile;
}

void CSceneLoader::StartLoading(vector<string>& _nameList, vector<string>& _fileList, vector<string>& _formatList)
{
	EnterCriticalSection(&GetInstance().m_pCriticalSection);

	GetInstance().m_mReadyFiles_Name = _nameList;
	GetInstance().m_mReadyFiles_Path = _fileList;
	GetInstance().m_mReadyFiles_Format = _formatList;

	GetInstance().m_iTootalFile = static_cast<_uint>(_fileList.size());
	GetInstance().m_iLoadedFile = 0;

	GetInstance().m_bLoading = true;

	LeaveCriticalSection(&GetInstance().m_pCriticalSection);
}

void CSceneLoader::ThreadLoadingLoop()
{
	while (GetInstance().m_bRunning)
	{
		EnterCriticalSection(&GetInstance().m_pCriticalSection);

		if (!GetInstance().m_mReadyFiles_Name.empty())
		{
			GetInstance().m_bLoading = true;

			string name = GetInstance().m_mReadyFiles_Name.back();
			string file = GetInstance().m_mReadyFiles_Path.back();
			string format = GetInstance().m_mReadyFiles_Format.back();

			GetInstance().m_mReadyFiles_Name.pop_back();
			GetInstance().m_mReadyFiles_Path.pop_back();
			GetInstance().m_mReadyFiles_Format.pop_back();

			LeaveCriticalSection(&GetInstance().m_pCriticalSection);

			wstring wName = CEngineString::StringToWString(name);
			wstring wFile = CEngineString::StringToWString(file);
			wstring wFormat = CEngineString::StringToWString(format);

			using namespace EngineAI;

			if (wName == L"NaviMesh_Walkable")
			{
				CNaviMesh::NaviMeshBufferInitiaizeInfo navInfo_able = CResources::ReadNaviBufferInfos(wFile, true);
				CResources::CreateNaviMesh(L"NaviMesh_Walkable", navInfo_able);
			}
			if (wName == L"NaviMesh_WalkUnable")
			{
				CNaviMesh::NaviMeshBufferInitiaizeInfo navInfo_unable = CResources::ReadNaviBufferInfos(wFile, false);
				CResources::CreateNaviMesh(L"NaviMesh_WalkUnable", navInfo_unable);
			}
			if (CEngineString::Contains(wFile, L".png") || CEngineString::Contains(wFile, L".jpg") || CEngineString::Contains(wFile, L".tga"))
			{
				if (CEngineString::Contains(wFormat, L"[Texture]"))
					CResources::LoadResourceComplete_Scene(CResources::CreateSceneResource<CTexture>(wName + L" (Texture)", wFile, nullptr, true));
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

					auto meshInfoList = CResources::ReadMeshBufferInfos(meshdataPath);

					CResources::CreateSceneMeshBundle(wName + L" (Mesh Buffer)", meshInfoList, filter, nullptr, true);

					CMeshBuffer * bufferOne = CMeshBuffer::Create();
					bufferOne->Initialize_Custom(meshInfoList[0], nullptr);

					CResources::AddSceneResource(wName + L" (Mesh Buffer)", bufferOne, true);
				}
				if (CEngineString::Contains(wFormat, L"[Skinned Mesh]"))
				{
					_int filter = FILTER_MESHBUFFER;

					if (CEngineString::Contains(wFormat, L"[Material]"))
						filter |= FILTER_MATERIAL;
					if (CEngineString::Contains(wFormat, L"[Texture]"))
						filter |= FILTER_TEXTURE;
					if (CEngineString::Contains(wFormat, L"[Bone]"))
						filter |= FILTER_BONE;

					auto skinnedDataSplit = CEngineString::Split(wFile, L"/");
					wstring skinnedDataFolder = skinnedDataSplit[skinnedDataSplit.size() - 2];
					wstring skinnedDataTail = skinnedDataSplit[skinnedDataSplit.size() - 1];
					wstring skinnedDataName = CEngineString::Split(skinnedDataTail, L".")[0];

					const wstring skinnedDataPath = skinnedDataFolder + L"_" + skinnedDataName + L".skinneddata";

					auto skinnedInfoList = CResources::ReadSkinnedBufferInfos(skinnedDataPath);

					CResources::CreateSceneSkinnedBundle(wName + L" (Mesh Buffer)", skinnedInfoList.initList, skinnedInfoList.skeletalList, filter, nullptr, true);
				}
				if (CEngineString::Contains(wFormat, L"[Animation Clip]"))
				{
					auto animationDataSplit = CEngineString::Split(wFile, L"/");
					wstring animationDataFolder = animationDataSplit[animationDataSplit.size() - 2];
					wstring animationDataTail = animationDataSplit[animationDataSplit.size() - 1];
					wstring animationDataName = CEngineString::Split(animationDataTail, L".")[0];

					const wstring animationdataPath = animationDataFolder + L"_" + animationDataName + L".animdata";

					auto animaitonInfoList = CResources::ReadAnimationClipBufferInfos(animationdataPath);

					CAnimationClip* newClip = CResources::CreateSceneResource<CAnimationClip>(wName + L" (Animation)", wFile, nullptr, true);
					CResources::LoadResourceComplete_Scene(newClip);

					if (animaitonInfoList.size() > 0)
						newClip->Initiailize_Custom(animaitonInfoList[0], nullptr);
				}
			}
			else if (CEngineString::Contains(wFile, L".mp3") || CEngineString::Contains(wFile, L".wav") || CEngineString::Contains(wFile, L".ogg"))
			{
				if (CEngineString::Contains(wFormat, L"[Audio Clip]"))
				{
					CResources::LoadResourceComplete_Scene(CResources::CreateSceneResource<CAudioClip>(wName + L" (Audio)", wFile, nullptr, true));
				}
			}
			else if (wFile == L"SkyBox")
			{
				CMeshBuffer::TERRAINBUFFERDESC terranDesc = FormatToTerrainDesc(wName, wFormat);
				CResources::LoadResourceComplete_Scene(CResources::CreateSceneResource<CMeshBuffer>(wName + L" (Terrain MeshBuffer)", wFile, &terranDesc, true));
			}
			else if (wFile == L"Terrain")
			{
				CMeshBuffer::TERRAINBUFFERDESC terranDesc = FormatToTerrainDesc(wName, wFormat);
				CResources::LoadResourceComplete_Scene(CResources::CreateSceneResource<CMeshBuffer>(wName + L" (Terrain MeshBuffer)", wFile, &terranDesc, true));
			}

			++GetInstance().m_iLoadedFile;
		}
		else
		{
			GetInstance().m_bLoading = false;
			LeaveCriticalSection(&GetInstance().m_pCriticalSection);
			Sleep(10);
		}
	}
}

void CSceneLoader::Shutdown()
{
	GetInstance().m_bRunning = false;

	WaitForSingleObject(GetInstance().m_hThread, INFINITE);
	CloseHandle(GetInstance().m_hThread);
	DeleteCriticalSection(&GetInstance().m_pCriticalSection);
}

CSkyBox::SKYBOXBUFFERDESC CSceneLoader::FormatToSkyBoxDesc(wstring _name, wstring _format)
{
	CSkyBox::SKYBOXBUFFERDESC terrainDesc = {};

	wstring texturePath = CEngineString::Erase(_format, L"[");
	texturePath = CEngineString::Erase(texturePath, L"]");

	CResources::LoadResourceComplete_Scene(CResources::CreateSceneResource<CTexture>(_name + L" - Terrain Height map (Texture)", texturePath, nullptr, true));

	terrainDesc.texture = _name + L" - Terrain Height map (Texture)";

	return terrainDesc;
}

CMeshBuffer::TERRAINBUFFERDESC CSceneLoader::FormatToTerrainDesc(wstring _name, wstring _format)
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
	CResources::LoadResourceComplete_Scene(CResources::CreateSceneResource<CTexture>(_name + L" - Terrain Height map (Texture)", tokens[5], nullptr, true));
	terrainDesc.heightMap = _name + L" - Terrain Height map (Texture)";

	return terrainDesc;
}
