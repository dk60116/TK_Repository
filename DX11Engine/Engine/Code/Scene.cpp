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
	, m_vCloneResourceList({})
	, m_mMeshBundleList({})
	, m_mTempMeshBundleList({})
	, m_mSkinnedBundleList({})
	, m_mSkinnedBoneList({})
	, m_mTempSkinnedBoneList({})
	, m_lObjectList({})
	, m_lCameraList({})
	, m_lCanvasList({})
	, m_pEditorCamera(nullptr)
	, m_iUniqueObjectCount(0)
	, m_pMeshDepthStencilState(nullptr)
	, m_pUIDepthStencilState(nullptr)
{
	m_strName = L"Scene";

	m_pDevice = CGraphicDevice::GetInstance().Get_Device();
	m_pContext = CGraphicDevice::GetInstance().Get_Context();

	m_pDevice->AddRef();
	m_pContext->AddRef();
}

CScene::~CScene()
{
	SceneRelease();
}

HRESULT CScene::Initialize()
{
	SceneRelease();

	m_iUniqueObjectCount = 0;

	m_mResourceList = m_mTempResourceList;
	m_mMeshBundleList = m_mTempMeshBundleList;
	m_mSkinnedBundleList = m_mTempSkinnedBundleList;
	m_mSkinnedBoneList = m_mTempSkinnedBoneList;

	for (TRAVERSAL_ITER(m_mTempMeshBundleList, it))
		(*it).second.clear();
	for (TRAVERSAL_ITER(m_mTempSkinnedBundleList, it))
		(*it).second.clear();
	for (TRAVERSAL_ITER(m_mTempSkinnedBoneList, it))
		(*it).second.clear();

	m_mTempResourceList.clear();
	m_mTempMeshBundleList.clear();
	m_mTempSkinnedBundleList.clear();
	m_mTempSkinnedBoneList.clear();

	D3D11_DEPTH_STENCIL_DESC depthDefaultDesc = {};
	depthDefaultDesc.DepthEnable = TRUE;
	depthDefaultDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDefaultDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDefaultDesc.StencilEnable = FALSE;

	if (FAILED(m_pDevice->CreateDepthStencilState(&depthDefaultDesc, &m_pMeshDepthStencilState)))
		return E_FAIL;

	D3D11_DEPTH_STENCIL_DESC depthDisabledDesc = {};
	depthDisabledDesc.DepthEnable = FALSE;
	depthDisabledDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledDesc.StencilEnable = FALSE;

	if (FAILED(m_pDevice->CreateDepthStencilState(&depthDisabledDesc, &m_pUIDepthStencilState)))
		return E_FAIL;

	CDebug::Log(CDebug::MemoryUseLog());

#ifndef _CLIENT_BUILD
	CEditor::GetInstance().Set_SelectedGameObject(nullptr);
	CEditor::GetInstance().MoveTo_SelectedGameObject(nullptr);
	CGameObject* ecObj = Add_GameObject(L"Editor Camera Object");
	m_pEditorCamera = ecObj->AddComponent<CEditorCamera>();
	m_pEditorCamera->Get_Transform()->Set_Position(CEditor::GetInstance().Get_EditorCamPositon());
	m_pEditorCamera->Get_Transform()->Set_Quaternion(CEditor::GetInstance().Get_EditorCamQuaternion());
#endif

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
	{
		if ((*it)->IsActive())
			(*it)->Start();
	}
}

void CScene::Update_Editor()
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
		(*it)->Update_Editor();

	if (CInput::GetInstance().GetKey_Editor(CONTROL))
	{
		if (CInput::GetInstance().GetKeyDown_Editor(S))
		{
			wstring path = L"BinaryAssets/Scene/" + m_strSceneName + L".scenedata";
			CResources::GetInstance().SaveSceneObjectTransformInfos(path, Convert_ObjectsTransformInfo());
		}
	}

	if (CInput::GetInstance().GetKey_Editor(CONTROL))
	{
		if (CInput::GetInstance().GetMouseButtonDown_Editor(0))
		{
			const vector2Int point = CInput::GetInstance().GetMousePos_Editor();
			CPhysics::Ray ray = m_pEditorCamera->ScreenPointToRay_Editor(point);

			auto hits = CPhysics::GetInstance().Raycast(ray);

			if (hits.size() <= 0)
				return;

			auto firstHit = hits[0];

			CDebug::LogError("Ray Origin & Dir");
			CDebug::LogError(ray.origin);
			CDebug::LogError(ray.dir);
			CDebug::LogError("HitPos");
			CDebug::LogError(firstHit.hitPos);
			CDebug::LogError(firstHit.object->Get_ObjectName());

			CGameObject* newObj = Add_GameObject(L"AddObj");
			CMeshRenderer* newRen = newObj->AddComponent<CMeshRenderer>();
			newRen->Get_MeshFilter()->Set_MeshBuffer(CResources::GetInstance().LoadOnGame<CMeshBuffer>(L"Cube (Mesh Buffer)"));
			newRen->Get_Transform()->Get_Transform()->Set_Position(firstHit.hitPos);
		}
	}
}

void CScene::Update()
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		if ((*it)->IsActive())
			(*it)->Update();
	}
}

void CScene::FixedUpdate()
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		if ((*it)->IsActive())
			(*it)->FixedUpdate();
	}
}

void CScene::LateUpdateEditor()
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		if ((*it)->IsActive())
			(*it)->LateUpdate_Editor();
	}
}

void CScene::LateUpdate()
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		if ((*it)->IsActive())
			(*it)->LateUpdate();
	}
}

void CScene::Render_Editor()
{
	ColorValue backgroudColor = ColorValue::gray(0.3f);

	CGraphicDevice::GetInstance().Clear_BackBuffer_View(&backgroudColor);
	CGraphicDevice::GetInstance().Clear_DepthStencil_View();

	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		if ((*it)->IsActive())
		{
			(*it)->OnPreCull_Editor();
			(*it)->OnPreRender_Editor();
			(*it)->Render_Editor();
			(*it)->OnPostRender_Editor();
		}

		(*it)->Render_Gizmo();
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
		if ((*it)->IsActive())
			(*it)->Render();
	}

	for (TRAVERSAL_ITER(m_lCameraList, it))
		(*it)->OnPreCull();
	for (TRAVERSAL_ITER(m_lCameraList, it))
		(*it)->OnPreRender();

	m_pContext->OMSetDepthStencilState(m_pMeshDepthStencilState, 0);

	for (TRAVERSAL_ITER(m_lCameraList, it))
	{
		if ((*it)->Get_GameObject()->IsActive() && (*it)->Get_Enable())
			(*it)->RenderMesh();
	}

	m_pContext->OMSetDepthStencilState(m_pUIDepthStencilState, 0);

	for (TRAVERSAL_ITER(m_lCameraList, it))
	{
		if ((*it)->Get_GameObject()->IsActive() && (*it)->Get_Enable())
			(*it)->RenderUI();
	}

	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		if ((*it)->IsActive())
			(*it)->OnPostRender();
	}
}

void CScene::SceneRelease()
{
	m_lCameraList.clear();
	m_lLightList.clear();
	m_lCanvasList.clear();

	for (TRAVERSAL_ITER(m_lObjectList, it))
		Safe_Release(*it);
	for (TRAVERSAL_ITER(m_mResourceList, it))
		Safe_Release((*it).second);
	for (TRAVERSAL_ITER(m_mMeshBundleList, it))
	{
		for (TRAVERSAL_ITER((*it).second, it1))
		{
			Safe_Release((*it1).meshBuffer);
			Safe_Release((*it1).material);
			Safe_Release((*it1).texture);
		}

		(*it).second.clear();
	}
	for (TRAVERSAL_ITER(m_mSkinnedBundleList, it))
	{
		for (TRAVERSAL_ITER((*it).second, it1))
		{
			Safe_Release((*it1).meshBuffer);
			Safe_Release((*it1).material);
			Safe_Release((*it1).texture);
		}

		(*it).second.clear();
	}
	for (TRAVERSAL_ITER(m_mSkinnedBoneList, it))
		(*it).second.clear();
	for (TRAVERSAL_ITER(m_vCloneResourceList, it))
		Safe_Release(*it);

	m_lObjectList.clear();
	m_mResourceList.clear();
	m_mMeshBundleList.clear();
	m_mSkinnedBundleList.clear();
	m_mSkinnedBoneList.clear();
	m_vCloneResourceList.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

void CScene::Set_SceneName(const wstring _name)
{
	m_strSceneName = _name;
}

const wstring& CScene::Get_SceneName() const
{
	return m_strSceneName;
}

vector<CScene::SCENETRANSFORMINFO> CScene::Convert_ObjectsTransformInfo() const
{
	vector<SCENETRANSFORMINFO> result = {};

	_uint i = 0;

	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		SCENETRANSFORMINFO info = {};

		CTransform* tf = (*it)->Get_Transform();

		info.objID = (*it)->m_iUniqueID;
		info.objName = (*it)->m_strGameObjectName;
		vector3 pos = tf->Get_LocalPosition();
		info.localPos = pos;
		const quaternion quat = tf->Get_LocalQuaternion();
		info.localQuaternion = quat;
		info.localScale = tf->Get_LocalScale();

		CRectTransform* rect = (*it)->GetComponent<CRectTransform>();

		info.isRect = rect ? true : false;

		if (info.isRect)
		{
			SCENERECTINFO rectInfo = {};

			rectInfo.anchoredPos = rect->Get_AnchoredPosition();
			rectInfo.widthHeight = rect->Get_WidthHeight();
			rectInfo.pivot = rect->Get_Pivot();
			rectInfo.anchorMin = rect->Get_Anchors().min;
			rectInfo.anchorMax = rect->Get_Anchors().max;

			info.rectInfo = rectInfo;
		}

		if (i > 0)
			result.push_back(info);

		++i;
	}

	return result;
}

void CScene::Bind_ObjectsTransform(const vector<SCENETRANSFORMINFO> _infoList)
{
	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		CGameObject* obj = *it;

		if (obj->m_iUniqueID == 0)
			continue;

		for (TRAVERSAL_ITER(_infoList, it1))
		{
			if ((*it1).objID == obj->m_iUniqueID)
			{
				CTransform* tf = obj->Get_Transform();

				if (!(*it1).isRect)
					tf->Set_LocalScale((*it1).localScale);
				else
				{
					CRectTransform* rect = obj->GetComponent<CRectTransform>();
					rect->Set_Pivot((*it1).rectInfo.pivot);
					rect->Set_AnchorsMin((*it1).rectInfo.anchorMin);
					rect->Set_AnchorsMax((*it1).rectInfo.anchorMax);
					rect->Set_WidthHeight((*it1).rectInfo.widthHeight);
				}

				tf->Set_LocalQuaternion((*it1).localQuaternion);
				tf->Set_LocalPosition((*it1).localPos);
			}
		}
	}
}

CEngineResource* CScene::Add_Resource(const wstring& _name, CEngineResource* _resource)
{
	if (!_resource)
		return nullptr;

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

	auto iter1 = m_mTempResourceList.find(_name);

	if (iter1 != m_mTempResourceList.end())
		return iter1->second;

	return nullptr;
}

vector<MeshBundle> CScene::Find_MeshInfoResource(const wstring& _name)
{
	auto iter = m_mMeshBundleList.find(_name);

	if (iter != m_mMeshBundleList.end())
		return iter->second;

	auto iter1 = m_mTempMeshBundleList.find(_name);

	if (iter1 != m_mTempMeshBundleList.end())
		return iter1->second;

	return {};
}

vector<SkinnedMeshBundle> CScene::Find_SkinnedMeshInfoResource(const wstring& _name)
{
	auto iter = m_mSkinnedBundleList.find(_name);

	if (iter != m_mSkinnedBundleList.end())
		return iter->second;

	auto iter1 = m_mTempSkinnedBundleList.find(_name);

	if (iter1 != m_mTempSkinnedBundleList.end())
		return iter1->second;

	return {};
}

vector<CSkinnedMeshBuffer::SKINNEDSKELETAL> CScene::Find_SkinnedBonesResource(const wstring& _name)
{
	auto iter = m_mSkinnedBoneList.find(_name);

	if (iter != m_mSkinnedBoneList.end())
		return iter->second;

	auto iter1 = m_mTempSkinnedBoneList.find(_name);

	if (iter1 != m_mTempSkinnedBoneList.end())
		return iter1->second;

	return {};
}

CEngineResource* CScene::Add_TempResource(const wstring& _name, CEngineResource* _resource)
{
	if (!_resource)
		return nullptr;

	m_mTempResourceList.emplace(_name, _resource);
	_resource->AddRef();

	return _resource;
}

void CScene::Add_MeshBundle(const wstring& _name, vector<MeshBundle> _resource)
{
	for (TRAVERSAL_ITER(_resource, it))
	{
		if ((*it).meshBuffer)
			(*it).meshBuffer->AddRef();
		if ((*it).material)
			(*it).material->AddRef();
		if ((*it).texture)
			(*it).texture->AddRef();
	}

	m_mMeshBundleList.emplace(_name, _resource);
}

void CScene::Add_SkinnedBundle(const wstring& _name, vector<SkinnedMeshBundle> _resource)
{
	for (TRAVERSAL_ITER(_resource, it))
	{
		if ((*it).meshBuffer)
			(*it).meshBuffer->AddRef();
		if ((*it).material)
			(*it).material->AddRef();
		if ((*it).texture)
			(*it).texture->AddRef();
	}

	m_mSkinnedBundleList.emplace(_name, _resource);
}

void CScene::Add_TempMeshBundle(const wstring& _name, vector<MeshBundle> _resource)
{
	for (TRAVERSAL_ITER(_resource, it))
	{
		if ((*it).meshBuffer)
			(*it).meshBuffer->AddRef();
		if ((*it).material)
			(*it).material->AddRef();
		if ((*it).texture)
			(*it).texture->AddRef();
	}

	m_mTempMeshBundleList.emplace(_name, _resource);
}

void CScene::Add_TempSkinnedBundle(const wstring& _name, vector<SkinnedMeshBundle> _resource)
{
	for (TRAVERSAL_ITER(_resource, it))
	{
		if ((*it).meshBuffer)
			(*it).meshBuffer->AddRef();
		if ((*it).material)
			(*it).material->AddRef();
		if ((*it).texture)
			(*it).texture->AddRef();
	}

	m_mTempSkinnedBundleList.emplace(_name, _resource);
}

void CScene::Add_SkinnedMeshBone(const wstring& _name, vector<CSkinnedMeshBuffer::SKINNEDSKELETAL> _resource)
{
	m_mSkinnedBoneList.emplace(_name, _resource);
}

void CScene::Add_TempSkinnedMeshBone(const wstring& _name, vector<CSkinnedMeshBuffer::SKINNEDSKELETAL> _resource)
{
	m_mTempSkinnedBoneList.emplace(_name, _resource);
}

CEngineResource* CScene::Add_CloneResourece(CEngineResource* _resource)
{
	if (!_resource)
		return nullptr;

	m_vCloneResourceList.push_back(_resource);
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

vector<CRenderer*> CScene::Get_MeshObjects()
{
	vector<CRenderer*> result = {};

	for (TRAVERSAL_ITER(m_lObjectList, it))
	{
		if (CRenderer* ren = (*it)->GetComponent<CRenderer>())
		{
			if ((*it)->m_iUniqueID != 0)
				result.push_back(ren);
		}
	}

	return result;
}

const CScene::LightSettings& CScene::Get_LightSetting()
{
	return m_sLightSettings;
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

const list<class CCamera*>& CScene::Get_CameraList()
{
	return m_lCameraList;
}

CCamera* CScene::Add_Camera(CCamera* _camera)
{
	if (!_camera)
		return nullptr;

	m_lCameraList.push_back(_camera);

	return m_lCameraList.back();
}

const list<CLight*>& CScene::Get_LightList()
{
	return m_lLightList;
}

CLight* CScene::Add_Light(CLight* _light)
{
	if (!_light)
		return nullptr;

	m_lLightList.push_back(_light);

	return m_lLightList.back();
}

CCanvas* CScene::Get_Canvas(const _int _index) const
{
	_int i = 0;

	if (m_lCanvasList.size() <= 0)
		return nullptr;

	for (TRAVERSAL_ITER(m_lCanvasList, it))
	{
		++i;

		if (_index == i)
			return (*it);
	}

	return m_lCanvasList.back();
}

const list<CCanvas*>& CScene::Get_CanvasList()
{
	return m_lCanvasList;
}

CCanvas* CScene::Add_Canvas(CCanvas* _canvas)
{
	if (!_canvas)
		return nullptr;

	m_lCanvasList.push_back(_canvas);
	m_lCanvasList.back()->AddRef();

	return m_lCanvasList.back();
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

const _uint CScene::Get_UniqueObjectCount() const
{
	return m_iUniqueObjectCount;
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
	vector<string> formatList;

	while (getline(file, line))
	{
		if (CEngineString::Contains(line, "//"))
			continue;

		if (CEngineString::Contains(line, ':'))
		{
			auto split = CEngineString::Split(line, " : ");

			string name = "";
			string filepath = "";
			string format = "";

			name = split[0];
			filepath = split[1];

			if (split.size() >= 3)
				format = split[2];

			if (!CResources::FileExists(filepath))
			{
				nameList.push_back(name);
				fileList.push_back(filepath);
				formatList.push_back(format);
				CDebug::Log("Add File: " + filepath + " (Name: " + name + ")");
			}
			else
				CDebug::LogWarnning("Failed Add File: " + filepath);
		}
		else
			CDebug::LogError("Invalid line format: " + line);
	}

	CSceneLoader::GetInstance().StartLoading(nameList, fileList, formatList);

	return S_OK;
}

ID3D11DepthStencilState* CScene::Get_MeshStencillState() const
{
	return m_pMeshDepthStencilState;
}

ID3D11DepthStencilState* CScene::Get_UIStencillState() const
{
	return m_pUIDepthStencilState;
}