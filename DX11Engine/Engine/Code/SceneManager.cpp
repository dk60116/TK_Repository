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
	, m_mTagFlags({})
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
	GetInstance().m_vLayerFlags.reserve(32);
	GetInstance().m_vLayerFlags.resize(32);

	Add_Layer(0, L"Default");
	Add_Layer(30, L"NaviMesh_Walkable");
	Add_Layer(31, L"NaviMesh_WalkUnable");

	Add_Tag(0, L"Untagged");
	
	return S_OK;
}

void CSceneManager::Release()
{
	GetInstance().m_pCrtScene = nullptr;

	for (TRAVERSAL_ITER(GetInstance().m_mSceneList, it))
	{
		(*it).second->SceneRelease();
		Safe_Release((*it).second);
	}

	GetInstance().m_mSceneList.clear();
}

CScene* CSceneManager::CreateScene(CScene* _newScene, wstring _name)
{
	CScene* newScene = dynamic_cast<CScene*>(_newScene);
	if (!newScene)
	{
		CDebug::LogError(L"Create Scene Fail: Invalid scene instance.");
		return nullptr;
	}
	newScene->Set_SceneName(_name);

	GetInstance().m_mSceneList.emplace(_name, newScene);

	newScene->AddRef();

	wstring filePath = L"../Assets/Scenes/" + _name + L".scene";

	if (!CResources::FileExists(filePath))
	{
		ofstream outFile(CEngineString::WStringToString(filePath));

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
	return GetInstance().m_pCrtScene;
}

CScene* CSceneManager::Get_TempScene()
{
	return GetInstance().m_pTempScene;
}

map<wstring, CScene*>& CSceneManager::Get_SceneList()
{
	return GetInstance().m_mSceneList;
}

const _bool CSceneManager::Is_Loading()
{
	return GetInstance().m_bLoading;
}

void CSceneManager::LoadScene(wstring _scene)
{
	if (GetInstance().m_bLoading)
	{
		CDebug::LogError("Load Scene Fail: Already loading Scene.");
		return;
	}

	auto iter = GetInstance().m_mSceneList.find(_scene);

	if (iter == GetInstance().m_mSceneList.end())
	{
		CDebug::LogError(L"Load Scene Fail: Not found Scene: " + _scene);
		return;
	}
	else
		CDebug::Log(L"Load scene start: " + _scene);

#ifndef  _CLIENT_BUILD
	if (GetInstance().m_pCrtScene)
		CEditor::Set_EditorCamTransform(Get_EditorCamera()->Get_Transform());
#endif

	GetInstance().m_pTempScene = iter->second;

	if (GetInstance().m_pTempScene)
	{
		GetInstance().m_pTempScene->PreLoadResources();
		GetInstance().m_bLoading = true;
	}
}

void CSceneManager::LoadScene(CScene* _scene)
{
	LoadScene(_scene->Get_SceneName());
}

void CSceneManager::LoadComplete()
{
	GetInstance().m_pCrtScene = nullptr;
	GetInstance().m_pCrtScene = GetInstance().m_pTempScene;
	GetInstance().m_pTempScene = nullptr;

	if (GetInstance().m_pCrtScene)
	{
		GetInstance().m_pCrtScene->Initialize();
		GetInstance().m_bLoading = false;
	}

	wstring file = GetInstance().m_pCrtScene->Get_SceneName() + L".scenedata";
	auto sceneTransformInfo = CResources::ReadSceneObjectTransformInfos(file);

	GetInstance().m_pCrtScene->Bind_ObjectsTransform(sceneTransformInfo);
}

void CSceneManager::Add_Layer(const _uint _index, const wstring& _name)
{
	GetInstance().m_vLayerFlags[_index] = { _index, 1u << _index, _name };
}

void CSceneManager::Add_Tag(const _uint _index, const wstring& _name)
{
	GetInstance().m_mTagFlags.emplace(_index, _name);
}

_uint CSceneManager::NameToLayer(const wstring& _name)
{
	for (LayerFlag layer : GetInstance().m_vLayerFlags)
	{
		if (layer.name == _name)
			return layer.value;
	} 

	return 99999;
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

_uint CSceneManager::LayerToIndex(const _uint _layer)
{
	for (TRAVERSAL_ITER(GetInstance().m_vLayerFlags, it))
	{
		if ((*it).value == _layer)
			return (*it).index;
	}

	return 0;
}

_uint CSceneManager::IndexToLayer(const _uint _layerIndex)
{
	for (TRAVERSAL_ITER(GetInstance().m_vLayerFlags, it))
	{
		if ((*it).index == _layerIndex)
			return (*it).value;
	}

	return 0;
}

_uint CSceneManager::NameToIndex(const wstring& _name)
{
	for (TRAVERSAL_ITER(GetInstance().m_vLayerFlags, it))
	{
		if ((*it).name == _name)
			return (*it).index;
	}

	return 0;
}

wstring CSceneManager::TagToName(const _uint _tag)
{
	return GetInstance().m_mTagFlags[_tag];
}

_uint CSceneManager::NameToTag(const wstring _name)
{
	for (auto& kv : GetInstance().m_mTagFlags)
	{
		if (kv.second == _name)
			return kv.first;
	}

	return 0;
}

const _bool CSceneManager::LayerMaskResult(const _uint _source, const _uint _mask)
{
	return _mask & _source;
}

const _bool CSceneManager::CompareTag(const _uint _tag, const wstring& _tagName)
{
	return _tagName == GetInstance().m_mTagFlags[_tag];
}

CCamera* CSceneManager::Get_EditorCamera()
{
	return GetInstance().m_pCrtScene->Get_EditorCamera();
}
