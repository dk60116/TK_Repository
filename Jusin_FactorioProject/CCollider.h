#pragma once

#include "CMonoBehaviour.h"

class CPhysics;

class CCollider :
    public CMonoBehaviour
{
public:
	enum ColliderShape { RECT, CIRCLE };

public:
	CCollider();
	~CCollider();

public:
	virtual void Awake() PURE;
	virtual void Start() PURE;
	virtual void Reset() PURE;
	virtual void Update() PURE;
	virtual void FixedUpdate() PURE;
	virtual void LateUpdate() PURE;
	virtual void OnEnable() PURE;
	virtual void OnDisable() PURE;
	virtual void Render(HDC _hDC) PURE;
	virtual void OnCollisionEnter(CCollider* _other) PURE;
	virtual void OnCollisionStay(CCollider* _other) PURE;
	virtual void OnCollisionExit(CCollider* _other) PURE;
	virtual void OnDestroy() PURE;
	virtual void OnMouseEnter() PURE;
	virtual void OnMouseOver() PURE;
	virtual void OnMouseExit() PURE;

public:
	void SetBody(CGameObject* _obj) { m_pBodyObject = _obj; }
	CGameObject* getBody() { return m_pBodyObject; }
	void SetPivot(const vector2 _pos) { m_v2Pivot = _pos; }
	const vector2& getPivot() { return m_v2Pivot; }
	void SetSize(const vector2 _size) { m_v2Size = _size; }
	const vector2& getSize() { return m_v2Size; }
	const ColliderShape& getShape() { return m_eShape; }
	void SetColor(const ColorValue _color) { m_sColor = _color; }
	void SetStatic(const bool _value) { m_bIsStatic = _value; }
	bool& isTrigger() { return m_bIsTrigger; }
	void SetTrigger(const bool _value) { m_bIsTrigger = _value; }
	void AddEnterCollider(CCollider* _col);
	void RemoveEnterCollider(CCollider* _col);

protected:
	CGameObject* m_pBodyObject;
	ColliderShape m_eShape;
	vector2 m_v2Pivot;
	vector2 m_v2Size;
	bool m_bIsStatic, m_bIsTrigger;
	list<CCollider*> m_lEnteredColliderList;

	ColorValue m_sColor;

	friend class CPhysics;
};

