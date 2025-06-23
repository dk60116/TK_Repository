#pragma once

#include "Object.h"

NS_BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public UObject
{
protected:
	CComponent();
	~CComponent();

public:
	virtual HRESULT Initialize();
	void ComponentRelease();

	virtual void Awake();
	virtual void Start();
	virtual void Update_Editor();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void LateUpdate_Editor();
	virtual void LateUpdate();

	virtual void OnMouseEnter();
	virtual void OnMouseOver();
	virtual void OnMouseExit();
	virtual void OnMouseDown();
	virtual void OnMouseDrag();
	virtual void OnMouseUp();

	//virtual void OnCollisionEnter();
	//virtual void OnCollisionStay();
	//virtual void OnCollisionExit();
	//virtual void OnTriggerEnter();
	//virtual void OnTriggerStay();
	//virtual void OnTriggerExit();

	//virtual void OnCollisionEnter2D();
	//virtual void OnCollisionStay2D();
	//virtual void OnCollisionExit2D();
	//virtual void OnTriggerEnter2D();
	//virtual void OnTriggerStay2D();
	//virtual void OnTriggerExit2D();

	virtual void OnPreCull_Editor();
	virtual void OnPreRender_Editor();
	virtual void Render_Editor();
	virtual void OnPostRender_Editor();

	virtual void OnPreCull();
	virtual void OnPreRender();
	virtual void Render();
	virtual void OnPostRender();

	virtual void OnEnable();
	virtual void OnDisable();
	virtual void OnDestroy();

	virtual void OnApplicationQuit();

public:
	const _bool Get_Enable() const;
	void Set_Enable(const _bool _enable);

	void Set_Object(class CGameObject* _gameObject);

protected:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;

protected:
	_bool m_bEnable;
	class CGameObject* m_pGameObject;
};

NS_END

