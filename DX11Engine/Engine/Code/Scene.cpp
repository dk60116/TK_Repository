#include "epch.h"
#include "Scene.h"
#include "EditorCamera.h"

CScene::CScene()
	: m_iSceneIndex(0)
	, m_pDevice(nullptr)
	, m_pContext(nullptr)
	, m_strSceneName(L"")
	, m_mResourceList({})
	, m_mTempResourceList({})
	, m_lObjectList({})
	, m_lCameraList({})
	, m_pEditorCamera(nullptr)
	, m_iUniqueObjectCount(0)
{
	m_pDevice = CGraphicDevice::GetInstance().Get_Device();
	m_pContext = CGraphicDevice::GetInstance().Get_Context();

	m_pDevice->AddRef();
	m_pContext->AddRef();
}

CScene::~CScene()
{
	SceneRelease();
}

HRESULT CScene::PreLoadResources()
{
	string path = "../Assets/Scenes/" + CEngineString::WStringToString(m_strSceneName) + ".scene";
	ifstream file(path);
	if (!file)
	{
		CDebug::LogError("Can not Open file");
		return E_FAIL;
	}

	string line;
	vector<string> nameList;
	vector<string> fileList;

	while (getline(file, line))
	{
		size_t pos = line.find(':');
		if (pos != string::npos)
		{
			string name = line.substr(0, pos);
			string filepath = line.substr(pos + 1);

			name = CEngineString::Trim(name);
			filepath = CEngineString::Trim(filepath);

			if (!CResources::FileExists(filepath))
			{
				nameList.push_back(name);
				fileList.push_back(filepath);
				CDebug::Log("Add File: " + filepath + " (Name: " + name + ")");
			}
			else
				CDebug::LogWarnning("Failed Add File: " + filepath);
		}
		else
			CDebug::LogError("Invalid line format: " + line);
	}

	CSceneLoader::GetInstance().StartLoading(nameList, fileList);

	return S_OK;
}

HRESULT CScene::Initialize()
{
	SceneRelease();

	m_iUniqueObjectCount = 0;

	m_mResourceList = m_mTempResourceList;

	m_mTempResourceList.clear();

	CDebug::Log(CDebug::MemoryUseLog());

#ifndef _CLIENT_BUILD
	CEditor::GetInstance().Set_SelectedGameObject(nullptr);
	CGameObject* ecObj = Add_GameObject(L"Editor Camera Object");
	m_pEditorCamera = ecObj->AddComponent<CEditorCamera>();
#endif

	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		if (FAILED((*it)->Initialize()))
			return E_FAIL;
		else
			(*it)->Awake();
	}

	CDebug::Log(L"Load scene Complete: " + m_strSceneName);

	return S_OK;
}

void CScene::Awake()
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
		(*it)->Awake();
}

void CScene::Start()
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
		(*it)->Start();
}

void CScene::Update_Editor()
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
		(*it)->Update_Editor();
}

void CScene::Update()
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
		(*it)->Update();
}

void CScene::FixedUpdate()
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
		(*it)->FixedUpdate();
}

void CScene::LateUpdateEditor()
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
		(*it)->LateUpdate_Editor();
}

void CScene::LateUpdate()
{
}

void CScene::Render_Editor()
{
	ColorValue backgroudColor = ColorValue::gray(0.3f);

	CGraphicDevice::GetInstance().Clear_BackBuffer_View(&backgroudColor);
	CGraphicDevice::GetInstance().Clear_DepthStencil_View();

	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		(*it)->OnPreCull();
		(*it)->OnPreRender();
		(*it)->Render_Editor();
		(*it)->OnPostRender();
	}
}

void CScene::Render_Game()
{
	ColorValue backgroudColor = ColorValue::black();

	if (Get_Camera())
		backgroudColor = Get_Camera()->Get_BackgroundColor();

	CGraphicDevice::GetInstance().Clear_BackBuffer_View(&backgroudColor);
	CGraphicDevice::GetInstance().Clear_DepthStencil_View();

	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		(*it)->OnPreCull();
		(*it)->OnPreRender();
		(*it)->Render();
		(*it)->OnPostRender();
	}
}

void CScene::SceneRelease()
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
		Safe_Release(*it);

	for (TRAVERSAL_ITER(m_mResourceList, it))
		Safe_Release((*it).second);

	m_lObjectList.clear();
	m_mResourceList.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

void CScene::Set_Name(const wstring _name)
{
	m_strSceneName = _name;
}

const wstring& CScene::Get_SceneName() const
{
	return m_strName;
}

CEngineResource* CScene::Add_Resource(const wstring& _name, CEngineResource* _resource)
{
	if (!_resource) return nullptr;

	auto [it, inserted] = m_mResourceList.try_emplace(_name, _resource);

	if (inserted)    
	{
		_resource->AddRef();
		return _resource;
	}
	else                   
	{
		Safe_Release(_resource);
		return it->second;
	}
}

CEngineResource* CScene::Find_Resource(const wstring& _name)
{
	auto iter = m_mResourceList.find(_name);

	if (iter != m_mResourceList.end())
		return iter->second;

	return nullptr;
}

CEngineResource* CScene::Add_TempResource(const wstring& _name, CEngineResource* _resource)
{
	if (!_resource)
		nullptr;

	m_mTempResourceList.emplace(_name, _resource);
	_resource->AddRef();

	return _resource;
}

CGameObject* CScene::Add_GameObject(wstring _name)
{
	CGameObject* newObj = new CGameObject(_name, m_pDevice, m_pContext);
	newObj->AddRef();

	newObj->m_iUniqueID = m_iUniqueObjectCount++;

	m_lObjectList.push_back(newObj);

	newObj->Set_ObjectName(_name);

	if (FAILED(m_lObjectList.back()->Initialize()))
	{
		Safe_Release(newObj);
		return nullptr;
	}

	newObj->Set_Scene(this);

	return newObj;
}

vector<CGameObject*> CScene::Get_RootObjects()
{
	vector<CGameObject*> result = {};

	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		if ((*it)->Get_Transform()->Is_Root())
		{
			if ((*it)->m_iUniqueID != 0)
				result.push_back(*it);
		}
	}

	return result;
}

CCamera* CScene::Get_Camera() const
{
	if (m_lCameraList.size() <= 0)
		return nullptr;

	return m_lCameraList.back();
}

CCamera* CScene::Get_Camera(const _int _index) const
{
	_int i = 0;

	if (m_lCameraList.size() <= 0)
		return nullptr;

	for (TRAVERSAL_ITER(m_lCameraList, it))
	{
		++i;

		if (_index == i)
			return (*it);
	}

	return m_lCameraList.back();
}

CCamera* CScene::Get_EditorCamera() const
{
	return m_pEditorCamera;
}

list<class CCamera*>& CScene::Get_CameraList()
{
	return m_lCameraList;
}

CCamera* CScene::Add_Camera(CCamera* _camera)
{
	m_lCameraList.push_back(_camera);

	return _camera;
}

CGameObject* CScene::Instantiate(CGameObject* _gameObject)
{
	CGameObject* newObj = new CGameObject(*_gameObject);
	newObj->AddRef();

	m_lObjectList.push_back(newObj);

	return newObj;
}

HRESULT CScene::SaveScene(const wstring& _filePath)
{
	return S_OK;
}
