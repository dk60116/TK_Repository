#include "Management.h"
#include "EngineEditor.h"

CManagement::CManagement()
	: m_pGraphicDev(nullptr)
	, m_pCrtScene(nullptr)
	, m_mSceneList()
	, m_pEditorCamObj(nullptr)
	, m_pEditorCam(nullptr)
{
	m_strName = L"Management";
}

CManagement::~CManagement()
{
	Destroy();
}

void CManagement::CreateScene(CScene* _newScene, const wstring _name)
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

	iter->second->EditorInit();
	iter->second->UpdateEditor();

	CEngineEditor::GetInstance().getWindow<CHierachyWindow>()->BuildTree();

	return S_OK;
}

void CManagement::EditorUpdate()
{
	m_pEditorCamObj->UpdateEditor();
}

void CManagement::Destroy()
{
	Safe_Release(m_pEditorCamObj);

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
	m_pEditorCamObj = new CGameObject(L"Editor Camera", m_pGraphicDev);
	m_pEditorCamObj->Awake();
	m_pEditorCamObj->getTransform().SetPosition(0.f, 3.5f, -5.f);
	m_pEditorCamObj->getTransform().SetLocalEulerAnglesX(35.f);
	m_pEditorCam = m_pEditorCamObj->AddComponent<CEditorCamera>();
	m_pEditorCam->AwakeEditor();
	m_pEditorCamObj->UpdateEditor();
}
