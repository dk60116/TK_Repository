#include "CScene.h"

CScene::CScene()
	: m_lObjectList()
	, m_vCameraList({})
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
	for (int i = Layer::DEFAULT; i < Layer::LAYER_END; ++i)
	{
		for (TRAVERSAL_ITER(m_lObjectList[i], it))
		{
			if ((*it)->isActive() && (*it)->isEnable())
				(*it)->Render();
		}
	}
}

void CScene::Release()
{
	for (int i = Layer::DEFAULT; i < Layer::LAYER_END; ++i)
	{
		for (TRAVERSAL_ITER(m_lObjectList[i], it))
		{
			(*it)->OnDestroy();
			Safe_Delete(*it);
		}

		m_lObjectList[i].clear();
	}
}

CGameObject* CScene::AddObject(LPDIRECT3DDEVICE9 _device, wstring _objName, Layer _layer)
{
	CGameObject* obj = new CGameObject(_objName, _device);
	obj->SetScene(this);
	m_lObjectList[_layer].push_back(obj);
	obj->Awake();

	return obj;
}

void CScene::UpdateAllCameraResolution()
{
	for (TRAVERSAL_ITER(m_vCameraList, it))
		(*it)->ResetAspectFromResolution();
}
