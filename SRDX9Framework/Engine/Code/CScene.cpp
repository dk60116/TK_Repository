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

void CScene::AddObject(CGameObject* _obj, Layer _layer)
{
	_obj->SetScene(this);
	m_lObjectList[_layer].push_back(_obj);
	m_lObjectList[_layer].back()->Awake();
}
