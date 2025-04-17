#pragma once

#include "UObject.h"
#include "Engine_Define.h"
#include "CComponent.h"
#include "CTransform.h"
#include "CCamera.h"
#include "CLight.h"
#include "CManagement.h"
#include "CScene.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject
	: public UObject
{
private:
	explicit CGameObject(const wstring _name, LPDIRECT3DDEVICE9 _graphicDev);
	explicit CGameObject(CGameObject& _rhs);
	virtual ~CGameObject();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void LateUpdate();
	virtual void Render();
	virtual void OnEnable();
	virtual void OnDisable();
	virtual void OnDestroy();

public:
	const wstring& getName() { return m_strName; }
	const _bool& isActive() { return m_bIsActive; }
	const _bool& isEnable() { return m_bIsEnable; }
	CScene* getScene() { return m_pScene; }
	void SetScene(CScene* _scene) { m_pScene = _scene; }

public:
	template<typename T>
	T* AddComponent();
	template<typename T>
	T* GetComponent();

	CTransform& getTransform() { return *m_pTransform; }

protected:
	CScene* m_pScene;
	wstring m_strName;
	_bool m_isClone;
	_bool m_bKill;
	_bool m_bIsActive, m_bIsEnable;
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	CTransform* m_pTransform;
	list<CComponent*> m_lComponentlist;

	friend class CScene;
};

template<typename T>
inline T* CGameObject::AddComponent()
{
	T* newComponent = dynamic_cast<T*>(CComponent::Create<T>());
	dynamic_cast<CComponent*>(newComponent)->SetObject(this);
	m_lComponentlist.push_back(newComponent);
	//newComponent->AddRef();
	m_lComponentlist.back()->Awake();

	if (dynamic_cast<CCamera*>(newComponent))
		m_pScene->AddCamera(dynamic_cast<CCamera*>(newComponent));

	if (dynamic_cast<CLight*>(newComponent))
		m_pScene->AddLight(dynamic_cast<CLight*>(newComponent));

	return newComponent;
}

template<typename T>
inline T* CGameObject::GetComponent()
{
	for (TRAVERSAL_ITER(m_lComponentlist, it))
	{
		if (T* component = dynamic_cast<T*>(*it))
			return component;
	}

	return nullptr;
}

END