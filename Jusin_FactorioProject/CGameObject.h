#pragma once

#include "CMonoBehaviour.h"
#include "CTransform.h"

class CScene;
class CCollider;

enum Layer { Default = 0, Tile, Ore, Structure, FieldItem, Dead, Shadow, Player, Monster, Blueprint, UI, END};

class CGameObject
{
public:
	CGameObject();
	CGameObject(CGameObject& _rhs);
	~CGameObject();

public:
	void Awake();
	void Start();
	void Update();
	void FixedUpdate();
	void LateUpdate();
	void Render(HDC _hDC);
	void OnCollisionEnter(CCollider* _other);
	void OnCollisionStay(CCollider* _other);
	void OnCollisionExit(CCollider* _other);
	void OnEnable();
	void OnDisable();
	void OnDestroy();
	
public:
	template <typename T>
	T* GetComponent()
	{
		for (TRAVERSAL_ITER(m_vComponentList, it))
		{
			if (T* component = dynamic_cast<T*>(*it))
				return component;
		}

		return nullptr;
	}

	template <typename T>
	T* GetComponentInChildren()
	{
		for (TRAVERSAL_ITER(m_lChildList, it))
		{
			for (TRAVERSAL_ITER((*it)->m_vComponentList, it2))
			{
				if (T* component = dynamic_cast<T*>(*it2))
					return component;
			}
		}

		return nullptr;
	}

public:
	virtual void MessageUpdate();

public:
	void SetName(const wstring _name) { m_strName = _name; }
	const wstring& getName() { return m_strName; }
	CTransform& getTransform() { return m_sTransform; }
	Layer& getLayer() { return m_eLayer; };
	void SetLayer(const Layer _layer) { m_eLayer = _layer; }

	CMonoBehaviour* AddComponent(CMonoBehaviour* _component);
	void Destroy();

	const bool& isActive() { return m_bActive; }
	void SetActive(const bool _enable);
	const bool& isEnable() { return m_bEnable; }
	void SetEnable(const bool _enable) { m_bEnable = _enable; }
	CGameObject* getParent() { return m_pParent; }
	void SetParent(CGameObject* _parent);
	list<CGameObject*> getChildList() { return m_lChildList; }
	CScene* getScene() { return m_pMyScene; }
	void SetScene(CScene* _scene) { m_pMyScene = _scene; }

	const bool& getKill() { return m_bKill; }

protected:
	Layer m_eLayer;
	wstring m_strName;
	bool m_bActive, m_bPrevActive;
	bool m_bEnable;
	CTransform m_sTransform;

	CScene* m_pMyScene;

	vector<CMonoBehaviour*> m_vComponentList;

	CGameObject* m_pParent;
	list<CGameObject*> m_lChildList;

	bool m_bKill;
};
