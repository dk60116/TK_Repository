#include "CScene.h"
#include "CTime.h"
#include "CInput.h"
#include "CManagement.h"
#include "CGameObject.h"

CScene::CScene()
	: m_pGraphicDev(nullptr)
	, m_lObjectList()
	, m_pEditorCamera(nullptr)
	, m_vCameraList({})
	, m_vLightList({})
	, m_sOptions({})
{
}

CScene::~CScene()
{
	Destroy();
}

void CScene::Awake()
{
	if (!m_pEditorCamera)
	{
		CGameObject* editorCamObj = AddObject(L"Editor Camera", Layer::DEFAULT);
		m_pEditorCamera = editorCamObj->AddComponent<CEditorCamera>();
		m_pEditorCamera->AddRef();
		m_pEditorCamera->getTransform().SetPositionZ(-10.f);
	}
}

void CScene::Start()
{
}

void CScene::Update()
{
	UpdateAllLight();

	for (int i = Layer::DEFAULT; i < Layer::LAYER_END; ++i)
	{
		for (TRAVERSAL_ITER(m_lObjectList[i], it))
		{
			if ((*it)->isActive() && (*it)->isEnable())
				(*it)->Update();
		}
	}
}

void CScene::FixedUpdate()
{
}

void CScene::LateUpdateEditor()
{
	
}

void CScene::LateUpdate()
{
	for (int i = Layer::DEFAULT; i < Layer::LAYER_END; ++i)
	{
		for (TRAVERSAL_ITER(m_lObjectList[i], it))
		{
			if ((*it)->isActive() && (*it)->isEnable())
				(*it)->LateUpdate();
		}
	}
}

void CScene::Render_CScene()
{
	if (!m_pEditorCamera)
		return;

	CCamera* editorCam = m_pEditorCamera; // 에디터 카메라 기준
	vector3 cam_pos = editorCam->getTransform().getPosition();
	vector3 cam_forward = editorCam->getTransform().getDirections().forward;

	vector<CGameObject*> sortedRenderList;

	for (int i = Layer::DEFAULT; i < Layer::LAYER_END; ++i)
	{
		for (auto& obj : m_lObjectList[i])
		{
			if (obj->isActive() && obj->isEnable())
				sortedRenderList.push_back(obj);
		}
	}

	// 깊이 기준 정렬
	sort(sortedRenderList.begin(), sortedRenderList.end(),
		[&](CGameObject* a, CGameObject* b)
		{
			float depthA = vector3::dot((a->getTransform().getPosition() - cam_pos), cam_forward);
			float depthB = vector3::dot((b->getTransform().getPosition() - cam_pos), cam_forward);
			return depthA > depthB;
		});

	for (auto& obj : sortedRenderList)
	{
		obj->RenderEditor();
	}
}

void CScene::Render_Game()
{
	if (m_vCameraList.empty())
		return;

	CCamera* mainCam = m_vCameraList.back(); // 마지막 카메라 기준
	vector3 cam_pos = mainCam->getTransform().getPosition();
	vector3 cam_forward = mainCam->getTransform().getDirections().forward;

	vector<CGameObject*> sortedRenderList;

	for (int i = Layer::DEFAULT; i < Layer::LAYER_END; ++i)
	{
		for (auto& obj : m_lObjectList[i])
		{
			if (obj->isActive() && obj->isEnable())
				sortedRenderList.push_back(obj);
		}
	}

	// 깊이 기준 정렬
	sort(sortedRenderList.begin(), sortedRenderList.end(),
		[&](CGameObject* a, CGameObject* b)
		{
			float depthA = vector3::dot((a->getTransform().getPosition() - cam_pos), cam_forward);
			float depthB = vector3::dot((b->getTransform().getPosition() - cam_pos), cam_forward);
			return depthA > depthB;
		});

	for (auto& obj : sortedRenderList)
	{
		obj->Render();
	}
}

void CScene::Destroy()
{
	Safe_Release(m_pEditorCamera);

	for (int i = Layer::DEFAULT; i < Layer::LAYER_END; ++i)
	{
		for (TRAVERSAL_ITER(m_lObjectList[i], it))
		{
			(*it)->OnDestroy();
			Safe_Release(*it);
		}

		m_lObjectList[i].clear();
	}
}

void CScene::UpdateEditor()
{
	m_pEditorCamera->UpdateEditor();
}

CGameObject* CScene::AddObject(wstring _objName, Layer _layer)
{
	CGameObject* obj = new CGameObject(_objName, CManagement::GetInstance().getGraphicDevice());
	obj->AddRef();
	obj->SetScene(this);
	m_lObjectList[_layer].push_back(obj);
	obj->Awake();

	return obj;
}

void CScene::AddCamera(CCamera* _cam)
{
	if (_cam)
		m_vCameraList.push_back(_cam);
}

void CScene::AddLight(CLight* _light)
{
	if (_light)
	{
		_light->SetIndex((DWORD)m_vLightList.size());
		m_vLightList.push_back(_light);
	}
}

void CScene::UpdateSceneCameraResolution(const vector2Int& _resolution)
{
	m_pEditorCamera->ResetAspectFromResolution(_resolution);
}

void CScene::UpdateAllCameraResolution(const vector2Int& _resolution)
{
	for (TRAVERSAL_ITER(m_vCameraList, it))
		(*it)->ResetAspectFromResolution(_resolution);
}

void CScene::UpdateAllLight()
{
	for (TRAVERSAL_ITER(m_vLightList, it))
		(*it)->Apply();
}
