#include "CManagement.h"

CManagement::CManagement()
	: m_pGraphicDev(nullptr)
	, m_pCrtScene(nullptr)
	, m_mSceneList()
{
}

CManagement::~CManagement()
{
	Destroy();
}

HRESULT CManagement::ChangeScene(wstring _scene)
{
	auto iter = m_mSceneList.find(_scene);
	
	if (iter == m_mSceneList.end())
		return E_FAIL;

	if (m_pCrtScene)
		m_pCrtScene->Destroy();

	m_pCrtScene = iter->second;
	iter->second->Awake();

	return S_OK;
}

void CManagement::Destroy()
{
	for (TRAVERSAL_ITER(m_mSceneList, it))
	{
		Safe_Release((*it).second);
		//Safe_Delete((*it).second);
	}

	m_mSceneList.clear();
}
