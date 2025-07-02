#include "epch.h"
#include "SceneManager.h"

CSceneManager::CSceneManager()
	: m_pCrtScene(nullptr)
	, m_pTempScene(nullptr)
	, m_mSceneList({})
	, m_bLoading(false)
{
}

CSceneManager::~CSceneManager()
{
	Release();
}

CSceneManager& CSceneManager::GetInstance()
{
	static CSceneManager inst;
	return inst;
}

HRESULT CSceneManager::Initialize()
{
	return S_OK;
}

void CSceneManager::Release()
{
	m_pCrtScene = nullptr;

	for (TRAVERSAL_ITER(m_mSceneList, it))
	{
		(*it).second->SceneRelease();
		Safe_Release((*it).second);
	}

	m_mSceneList.clear();
}

CScene* CSceneManager::CreateScene(CScene* _newScene, wstring _name)
{
	CScene* newScene = dynamic_cast<CScene*>(_newScene);
	newScene->Set_Name(_name);

	m_mSceneList.emplace(_name, newScene);

	newScene->AddRef();

	return newScene;
}

CScene* CSceneManager::Get_CrtScene()
{
	return m_pCrtScene;
}

CScene* CSceneManager::Get_TempScene()
{
	return m_pTempScene;
}

const _bool CSceneManager::Is_Loading() const
{
	return m_bLoading;
}

void CSceneManager::LoadScene(wstring _scene)
{
	if (m_bLoading)
	{
		CDebug::LogError("Load Scene Fail: Already loading Scene.");
		return;
	}

	CDebug::Log(L"Load scene start: " + _scene);

	auto iter = m_mSceneList.find(_scene);

	if (iter == m_mSceneList.end())
		return;

	m_pTempScene = iter->second;

	if (m_pTempScene)
	{
		m_pTempScene->PreLoadResources();
		m_bLoading = true;
	}
}

void CSceneManager::LoadComplete()
{
	m_pCrtScene = nullptr;
	m_pCrtScene = m_pTempScene;
	m_pTempScene = nullptr;

	if (m_pCrtScene)
	{
		m_pCrtScene->Initialize();
		m_bLoading = false;
	}
}
