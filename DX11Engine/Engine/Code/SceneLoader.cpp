#include "epch.h"
#include "SceneLoader.h"

CSceneLoader::CSceneLoader()
	: m_hThread(nullptr)
	, m_pCriticalSection()
	, m_mReadyFiles({})
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

void CSceneLoader::StartLoading(vector<string>& _nameList, vector<string>& _fileList)
{
	EnterCriticalSection(&m_pCriticalSection);

	for (_int i = 0; i < _fileList.size(); ++i)
		m_mReadyFiles.emplace(_nameList[i], _fileList[i]);

	m_bLoading = true;

	LeaveCriticalSection(&m_pCriticalSection);
}

void CSceneLoader::ThreadLoadingLoop()
{
	while (m_bRunning)
	{
		EnterCriticalSection(&m_pCriticalSection);

		if (!m_mReadyFiles.empty())
		{
			m_bLoading = true;

			// 맵의 첫 번째 요소 반복자
			auto iter = m_mReadyFiles.begin();

			string name = iter->first;
			string file = iter->second;

			// 맵에서 제거
			m_mReadyFiles.erase(iter);

			LeaveCriticalSection(&m_pCriticalSection);

			wstring wName = CEngineString::StringToWString(name);
			wstring path = CEngineString::StringToWString(file);

			for (size_t i = 0; i < 1; i++)
			{
				if (path.find(L".png") != wstring::npos)
				{
					CResources::GetInstance().CreateResource<CTexture>(wName + L" (Texture)", path, nullptr, true);
				}
				else if (path.find(L".fbx") != wstring::npos)
				{
					CResources::GetInstance().CreateResource<CMeshBuffer>(wName + L" (MeshBuffer)", path, nullptr, true);
					CResources::GetInstance().CreateResource<CAnimation>(wName + L" (Animation)", path, nullptr, true);
				}
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
