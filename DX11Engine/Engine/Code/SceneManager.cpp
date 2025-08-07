#include "epch.h"
#include "SceneManager.h"

CSceneManager::CSceneManager()
	: m_pCrtScene(nullptr)
	, m_pTempScene(nullptr)
	, m_mSceneList({})
	, m_bLoading(false)
	, m_pEditorCamObj(nullptr)
	, m_pEditorCamera(nullptr)
	, m_vLayerFlags({})
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
	m_vLayerFlags.reserve(32);
	m_vLayerFlags.resize(32);

	Add_Layer(0, L"Default");
	Add_Layer(30, L"NaviMesh_Walkable");
	Add_Layer(31, L"NaviMesh_WalkUnable");
	
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
	newScene->Set_SceneName(_name);

	m_mSceneList.emplace(_name, newScene);

	newScene->AddRef();

	string filePath = "../Assets/Scenes/" + CEngineString::WStringToString(_name) + ".scene";

	if (!CResources::FileExists(filePath))
	{
		ofstream outFile(filePath);

		if (outFile.is_open())
		{
			outFile << "SceneName : " << CEngineString::WStringToString(_name);
			outFile.close();
		}
	}

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

const map<wstring, CScene*>& CSceneManager::Get_SceneList()
{
	return m_mSceneList;
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

	auto iter = m_mSceneList.find(_scene);

	if (iter == m_mSceneList.end())
	{
		CDebug::LogError(L"Load Scene Fail: Not found Scene: " + _scene);
		return;
	}
	else
		CDebug::Log(L"Load scene start: " + _scene);

#ifndef  _CLIENT_BUILD
	if (m_pCrtScene)
		CEditor::GetInstance().Set_EditorCamTransform(Get_EditorCamera()->Get_Transform());
#endif

	m_pTempScene = iter->second;

	if (m_pTempScene)
	{
		m_pTempScene->PreLoadResources();
		m_bLoading = true;
	}
}

void CSceneManager::LoadScene(CScene* _scene)
{
	LoadScene(_scene->Get_SceneName());
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

	wstring file = m_pCrtScene->Get_SceneName() + L".scenedata";
	auto sceneTransformInfo = CResources::GetInstance().ReadSceneObjectTransformInfos(file);

	m_pCrtScene->Bind_ObjectsTransform(sceneTransformInfo);
	m_pCrtScene->Awake();
	m_pCrtScene->Start();
}

void CSceneManager::Add_Layer(const _uint _index, const wstring _name)
{
	m_vLayerFlags[_index] = { 1u << _index, _name };
}

_uint CSceneManager::NameToLayer(const wstring _name)
{
	for (LayerFlag layer : GetInstance().m_vLayerFlags)
	{
		if (layer.name == _name)
			return layer.index;
	} 

	return 0;
}

wstring CSceneManager::LayerToName(const _uint _layer)
{
	if (_layer == 0)
		return L"";

	_uint index = 0;
	_uint mask = _layer;

	while ((mask & 1) == 0)
	{
		mask >>= 1;
		++index;
	}

	if (index >= GetInstance().m_vLayerFlags.size())
		return L"";

	return GetInstance().m_vLayerFlags[index].name;
}

_bool CSceneManager::LayerMaskResult(const _uint _source, const _uint _mask)
{
	return _mask & _source;
}

CCamera* CSceneManager::Get_EditorCamera()
{
	return m_pCrtScene->Get_EditorCamera();
}
