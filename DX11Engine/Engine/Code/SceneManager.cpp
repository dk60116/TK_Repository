#include "epch.h"
#include "SceneManager.h"

CSceneManager::CSceneManager()
	: m_pCrtScene(nullptr)
	, m_mSceneList({})
{
}

CSceneManager::~CSceneManager()
{
	Release();
}

void CSceneManager::Release()
{
	m_pCrtScene = nullptr;

	for (TRAVERSAL_ITER(m_mSceneList, it))
	{
		(*it).second->Release();
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

void CSceneManager::LoadScene(wstring _scene)
{
	auto iter = m_mSceneList.find(_scene);

	if (m_pCrtScene)
		m_pCrtScene->SceneRelease();

	m_pCrtScene = nullptr;

	m_pCrtScene = iter->second;

	m_pCrtScene->Initialize();
}
