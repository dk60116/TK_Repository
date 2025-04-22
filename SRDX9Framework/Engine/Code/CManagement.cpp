#include "CManagement.h"

CManagement::CManagement()
	: m_pGraphicDev(nullptr)
	, m_pCrtScene(nullptr)
	, m_mSceneList()
	, m_pEditorCam(nullptr)
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

	m_pGraphicDev->AddRef();

	m_mSceneList.insert({ _name, _newScene });
	_newScene->AddRef();

	CreateEditorCamera();
}

HRESULT CManagement::LoadScene(wstring _scene)
{
	auto iter = m_mSceneList.find(_scene);
	
	if (iter == m_mSceneList.end())
		return E_FAIL;

	if (m_pCrtScene)
		m_pCrtScene->SceneRelease();

	m_pCrtScene = nullptr;

	m_pCrtScene = iter->second;

	iter->second->Awake();
	iter->second->UpdateEditor();
	iter->second->Update();

	return S_OK;
}

void CManagement::EditorUpdate()
{
	m_cEditorCamObj->UpdateEditor();
}

void CManagement::Destroy()
{
	Safe_Release(m_cEditorCamObj);

	for (TRAVERSAL_ITER(m_mSceneList, it))
	{
		(*it).second->Release();
		Safe_Release((*it).second);
	}

	m_mSceneList.clear();
}

void CManagement::SetGraphicDevice(LPDIRECT3DDEVICE9 _gd)
{
	m_pGraphicDev = _gd;
	m_pGraphicDev->AddRef();
}

void CManagement::CreateEditorCamera()
{
	m_cEditorCamObj = new CGameObject(L"Editor Camera", m_pGraphicDev);
	m_cEditorCamObj->Awake();
	m_cEditorCamObj->getTransform().SetPosition(0.f, 3.5f, -5.f);
	m_cEditorCamObj->getTransform().SetLocalEulerAnglesX(35.f);
	m_pEditorCam = m_cEditorCamObj->AddComponent<CEditorCamera>();
	m_pEditorCam->AwakeEditor();
	m_cEditorCamObj->UpdateEditor();
}
