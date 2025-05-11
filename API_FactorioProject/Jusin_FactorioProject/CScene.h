#pragma once

#include "CMonoBehaviour.h"
#include "pch.h"
#include "CGameObject.h"
#include "CCamera.h"

class CScene
{
public:
	CScene();
	~CScene();

public:
	virtual void Awake() PURE;
	virtual void Start() PURE;
	virtual void Update() PURE;
	virtual void FixedUpdate() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render(HDC _hdc) PURE;
	virtual void Release() PURE;

public:
	void Awake_();
	void Start_();
	void Update_();
	void FixedUpdate_();
	void LateUpdate_();
	void Render_(HDC _hDC);
	void Release_();

public:
	template <typename T>
	T* FindObjectOfType(Layer _layer)
	{
		for (auto it = m_lObjList[_layer].begin(); it != m_lObjList[_layer].end(); ++it)
		{
			T* result = (*it)->GetComponent<T>();

			if (result != nullptr)
				return result;
		}

		return nullptr;
	}

public:
	CGameObject* AddObject(wstring _name, Layer _layer);
	CGameObject* SearchObject(wstring _name);
	CGameObject* Instantiate(CGameObject* _obj, wstring _name);
	CGameObject* Instantiate(CGameObject* _obj, wstring _name, vector2 _pos);

public:
	wstring& getName() { return m_strName; }

	CCamera* getMainCamera();
	void AddCamera(CCamera* _camera);

protected:
	wstring m_strName;

	list<CGameObject*> m_lObjList[Layer::END];

	list<CCamera*> m_lCameraList;
};

