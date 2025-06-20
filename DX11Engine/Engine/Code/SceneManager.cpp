#include "epch.h"
#include "SceneManager.h"

CSceneManager::CSceneManager()
	: m_pCrtScene(nullptr)
	, m_lSceneList({})
{
}

CSceneManager::~CSceneManager()
{
	Destroy();
}

void CSceneManager::Destroy()
{
	m_pCrtScene = nullptr;

	for (TRAVERSAL_ITER(m_lSceneList, it))
	{
		(*it)->SceneRelease();
		Safe_Release(*it);
	}
}

CScene* CSceneManager::Get_CrtScene()
{
	return m_pCrtScene;
}
