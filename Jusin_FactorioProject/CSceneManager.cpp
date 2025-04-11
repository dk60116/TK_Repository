#include "CSceneManager.h"
#include "CMianScene.h"
#include "CTestScene.h"
#include "CMainProcess.h"
#include "CControlManager.h"
#include "CPhysics.h"

extern wstring g_strNextScene;

CSceneManager::CSceneManager()
	: m_pCurrentScene(nullptr)
	, m_mSceneList({})
{

}

CSceneManager::~CSceneManager()
{
	Release();
}

void CSceneManager::Init()
{
	AddScene(new CMianScene());
	AddScene(new CTestScene());
}

void CSceneManager::Release()
{
	for (TRAVERSAL_ITER(m_mSceneList, it))
	{
		(*it).second->Release();
		Safe_Delete((*it).second);
	}

	m_mSceneList.clear();
}

void CSceneManager::AddScene(CScene* _scene)
{
	m_mSceneList.insert({_scene->getName() , _scene});
}

void CSceneManager::LoadScene(wstring _scene)
{
	g_strNextScene = _scene;
}

void CSceneManager::ChangeScene(wstring _scene)
{
	if (m_pCurrentScene != nullptr)
		m_pCurrentScene->Release();

	CControlManager::GetInstance().Release();
	CPhysics::GetInstance().Release();
	m_pCurrentScene = m_mSceneList[_scene];
	m_pCurrentScene->Awake();
	m_pCurrentScene->Start();
}
