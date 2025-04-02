#pragma once

#include "CMonoBehaviour.h"
#include "pch.h"
#include "CTransform.h"

class CGameObject;
class CCollider;

class CMonoBehaviour
{
public:
	CMonoBehaviour();
	virtual ~CMonoBehaviour();

public:
	virtual void Awake() PURE;
	virtual void Start() PURE;
	virtual void Reset() PURE;
	virtual void Update() PURE;
	virtual void FixedUpdate() PURE;
	virtual void LateUpdate() PURE;
	virtual void OnEnable() PURE;
	virtual void OnDisable() PURE;
	virtual void Render(HDC _hdc) PURE;
	virtual void OnCollisionEnter(CCollider* _other) PURE;
	virtual void OnCollisionStay(CCollider* _other) PURE;
	virtual void OnCollisionExit(CCollider* _other) PURE;
	virtual void OnDestroy() PURE;
	virtual void OnMouseEnter() PURE;
	virtual void OnMouseOver() PURE;
	virtual void OnMouseExit() PURE;

public:
	virtual void MessageUpdate();

public:
	void Awake_();
	void Start_();
	void Reset_();
	void Update_();
	void FixedUpdate_();
	void LateUpdate_();
	void OnEnable_();
	void OnDisable_();
	void OnCollisionEnter_(CCollider* _other);
	void OnCollisionStay_(CCollider* _other);
	void OnCollisionExit_(CCollider* _other);
	void OnDestroy_();

public:
	const bool& getEnable() { return  m_bEnable; }
	void SetEnable(const bool _enable) { m_bEnable = _enable; }
	CGameObject* getObj() { return m_pGameObject; }
	void SetObject(CGameObject* _obj) { m_pGameObject = _obj; }
	CTransform& getTransform();

protected:
	bool m_bEnable;
	CGameObject* m_pGameObject;
	bool m_bIsMouse, m_bIsMousePrev;

public:
	static std::string WStringToString(const std::wstring& wstr)
	{
		if (wstr.empty()) return std::string();

		int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
		std::string str(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str[0], size_needed, NULL, NULL);
		return str;
	}
};

