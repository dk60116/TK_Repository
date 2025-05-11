#include "CScene.h"
#include "CSceneManager.h"
#include "CInput.h"
#include "CMapManager.h"
#include "CPhysics.h"

CScene::CScene()
	: m_strName(L"")
	, m_lObjList()
{
}

CScene::~CScene()
{
	Release_();
}

CGameObject* CScene::AddObject(wstring _name, Layer _layer)
{
	CGameObject& _newObj = *(new CGameObject());

	_newObj.SetScene(this);
	_newObj.SetLayer(_layer);
	_newObj.SetName(_name);

	m_lObjList[_layer].push_back(&_newObj);

	return m_lObjList[_layer].back();
}

CGameObject* CScene::SearchObject(wstring _name)
{
	for (size_t i = Layer::Default; i < Layer::END; ++i)
	{
		for (TRAVERSAL_ITER(m_lObjList[i], it))
		{
			if ((*it)->getName() == _name)
				return (*it);
		}
	}

	return nullptr;
}

CGameObject* CScene::Instantiate(CGameObject* _obj, wstring _name)
{
	CGameObject* newObj = _obj;

	newObj->SetName(_name);

	m_lObjList[_obj->getLayer()].push_back(newObj);

	return m_lObjList[_obj->getLayer()].back();
}

CGameObject* CScene::Instantiate(CGameObject* _obj, wstring _name, vector2 _pos)
{
	CGameObject* newObj = _obj;

	newObj->SetName(_name);
	newObj->getTransform().SetPosition(_pos);

	m_lObjList[_obj->getLayer()].push_back(newObj);

	return m_lObjList[_obj->getLayer()].back();
}

CCamera* CScene::getMainCamera()
{
	if (m_lCameraList.size() <= 0)
		return nullptr;

	return m_lCameraList.front();
}

void CScene::AddCamera(CCamera* _camera)
{
	m_lCameraList.push_back(_camera);
}

void CScene::Awake_()
{
	for (size_t i = Layer::Default; i < Layer::END; ++i)
	{
		for (TRAVERSAL_ITER(m_lObjList[i], it))
			(*it)->Awake();
	}

	Start();
}

void CScene::Start_()
{
	for (size_t i = Layer::Default; i < Layer::END; ++i)
	{
		for (TRAVERSAL_ITER(m_lObjList[i], it))
			(*it)->Start();
	}
}

void CScene::Update_()
{
	for (size_t i = Layer::Default; i < Layer::END; ++i)
	{
		if (i == Layer::Tile)
		{
			for (TRAVERSAL_ITER(m_lObjList[i], it))
			{
				(*it)->Update();

				if (CMapManager::GetInstance().getMessageQueue().size() > 0)
				{
					(*it)->MessageUpdate();
					CMapManager::CMapManager::GetInstance().getMessageQueue().pop();
				}
			}
		}
		else
		{
			for (TRAVERSAL_ITER(m_lObjList[i], it))
			{
				if ((*it)->isEnable())
					(*it)->Update();
			}
		}
	}
}

void CScene::FixedUpdate_()
{
	for (size_t i = Layer::Default; i < Layer::END; ++i)
	{
		if (i == Layer::Tile)
			continue;

		for (TRAVERSAL_ITER(m_lObjList[i], it))
		{			
			if ((*it)->isActive() && (*it)->isEnable())
				(*it)->FixedUpdate();
		}
	}
}

void CScene::LateUpdate_()
{
	for (size_t i = Layer::Default; i < Layer::END; ++i)
	{
		if (i == Layer::Tile)
			continue;

		for (TRAVERSAL_ITER(m_lObjList[i], it))
			(*it)->LateUpdate();

		for (auto it = m_lObjList[i].begin(); it != m_lObjList[i].end();)
		{
			if ((*it)->getKill())
			{
				Safe_Delete(*it);
				it = m_lObjList[i].erase(it);
			}
			else
				++it;
		}
	}
}

void CScene::Render_(HDC _hDC)
{
	for (size_t i = Layer::Default; i < Layer::END; ++i)
	{
		for (TRAVERSAL_ITER(m_lObjList[i], it))
		{
			if ((*it)->isEnable())
				(*it)->Render(_hDC);
		}
	}
}

void CScene::Release_()
{
	for (size_t i = Layer::Default; i < Layer::END; ++i)
	{
		for (TRAVERSAL_ITER(m_lObjList[i], it))
		{
			(*it)->OnDestroy();
			Safe_Delete(*it);
		}

		m_lObjList[i].clear();
	}

	m_lCameraList.clear();
}
