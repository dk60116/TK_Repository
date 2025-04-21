#pragma once

#include "UObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CGameObject;
class CTransform;

class ENGINE_DLL CComponent abstract
	: public UObject
{
public:
	explicit CComponent();
	explicit CComponent(const CComponent& _rhs);
	~CComponent();

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
	CGameObject* getObject() { return m_pGameObject; }
	void SetDevice();
	const _bool& IsEnable() { return m_bEnable; };
	void SetObject(CGameObject* _obj) { m_pGameObject = _obj; }
	CTransform& getTransform();

protected:
	CGameObject* m_pGameObject;
	_bool m_bEnable;
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	_bool m_bClone;
};

END
