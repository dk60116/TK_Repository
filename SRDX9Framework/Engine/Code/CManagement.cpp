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

void CManagement::CreateScene(CScene* _newScene, wstring _name)
{
	_newScene->SetName(_name);
	_newScene->SetGraphicDev(m_pGraphicDev);

	m_mSceneList.insert({ _name, _newScene });
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
		Safe_Delete((*it).second);
	}

	m_mSceneList.clear();
}

void CManagement::SetGraphicDevice(LPDIRECT3DDEVICE9 _gd)
{
	m_pGraphicDev = _gd;
	//m_pGraphicDev->AddRef();
}
