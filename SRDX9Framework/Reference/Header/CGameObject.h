#pragma once

#include "UObject.h"
#include "Engine_Define.h"
#include "CComponent.h"
#include "CTransform.h"
#include "CEditorCamera.h"
#include "CLight.h"
#include "CManagement.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject final
	: public UObject
{
	friend class CManagement;
	friend class CScene;

private:
	explicit CGameObject(wstring _name, LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CGameObject(const CGameObject& _rhs);
	virtual ~CGameObject();

public:
	virtual void Awake();
	virtual void Start();
	virtual void UpdateEditor();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void LateUpdate();
	virtual void RenderEditor();
	virtual void Render();
	virtual void OnEnable();
	virtual void OnDisable();
	virtual void OnDestroy();

public:
	const wstring& getName() const { return m_strGameObjectName; }
	const _bool& isActive() { return m_bIsActive; }
	const _bool& isEnable() { return m_bIsEnable; }
	CScene* getScene() { return m_pScene; }
	void SetScene(CScene* _scene) { m_pScene = _scene; }
	void DestroyThis();

public:
	template<typename T>
	T* AddComponent();
	template<typename T>
	T* GetComponent();

	CTransform& getTransform() { return *m_pTransform; }

public:
	static CGameObject* Find(const wstring _name);
	static void Destroy(CGameObject* _object);

protected:
	CScene* m_pScene;
	wstring m_strGameObjectName;
	_bool m_isClone;
	_bool m_bKill;
	_bool m_bIsActive, m_bIsEnable;
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	CTransform* m_pTransform;
	list<CComponent*> m_lComponentlist;
};

template<typename T>
inline T* CGameObject::AddComponent()
{
	T* newComponent = T::Create();
	dynamic_cast<CComponent*>(newComponent)->SetObject(this);
	newComponent->AddRef();
	m_lComponentlist.push_back(newComponent);
	newComponent->Awake();

	if (dynamic_cast<CCamera*>(newComponent))
	{
		if (!dynamic_cast<CEditorCamera*>(newComponent))
			m_pScene->AddCamera(dynamic_cast<CCamera*>(newComponent));
	}

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