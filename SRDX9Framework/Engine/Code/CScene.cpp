#include "CScene.h"
#include "CManagement.h"
#include "CGameObject.h"

CScene::CScene()
	: m_pGraphicDev(nullptr)
	, m_lObjectList()
	, m_vCameraList({})
	, m_vLightList({})
	, m_sOptions({})
{
}

CScene::~CScene()
{
	Release();
}

void CScene::Awake()
{
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

void CScene::LateUpdate()
{
}

void CScene::Render()
{
	//for (int i = Layer::DEFAULT; i < Layer::LAYER_END; ++i)
	//{
	//	for (TRAVERSAL_ITER(m_lObjectList[i], it))
	//	{
	//		if ((*it)->isActive() && (*it)->isEnable())
	//			(*it)->Render();
	//	}
	//}

	if (m_vCameraList.empty())
		return;

	CCamera* mainCam = m_vCameraList[0]; // 첫 번째 카메라 기준
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

void CScene::Release()
{
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

CGameObject* CScene::AddObject(wstring _objName, Layer _layer)
{
	CGameObject* obj = new CGameObject(_objName, CManagement::GetInstance().getGraphicDevice());
	obj->SetScene(this);
	obj->AddRef();
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

void CScene::UpdateAllCameraResolution()
{
	for (TRAVERSAL_ITER(m_vCameraList, it))
		(*it)->ResetAspectFromResolution();
}

void CScene::UpdateAllLight()
{
	for (TRAVERSAL_ITER(m_vLightList, it))
		(*it)->Apply();
}
