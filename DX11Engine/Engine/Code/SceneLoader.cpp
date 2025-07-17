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
					CResources::LoadComplete_Scene(CResources::GetInstance().CreateSceneResource<CTexture>(wName + L" (Texture)", wFile, nullptr, true));
			}
			else if (CEngineString::Contains(wFile, L".fbx"))
			{
				if (CEngineString::Contains(wFormat, L"[Mesh]"))
				{
					_int filter = MESHBUFFER;

					if (CEngineString::Contains(wFormat, L"[Material]"))
						filter |= MATERIAL;
					if (CEngineString::Contains(wFormat, L"[Texture]"))
						filter |= TEXTURE;

					CResources::GetInstance().CreateSceneMeshBundle(wName + L" (MeshBuffer)", wFile, filter, nullptr, true);
				}
				if (CEngineString::Contains(wFormat, L"[Skinned Mesh]"))
					CResources::LoadComplete_Scene(CResources::GetInstance().CreateSceneResource<CSkinnedMeshBuffer>(wName + L" (Skinned MeshBuffer)", wFile, nullptr, true));
				if (CEngineString::Contains(wFormat, L"[Animation Clip]"))
					CResources::LoadComplete_Scene(CResources::GetInstance().CreateSceneResource<CAnimationClip>(wName + L" (Animation)", wFile, nullptr, true));
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
