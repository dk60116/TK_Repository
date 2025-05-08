#pragma once

#include "UObject.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CGameObject;
class CTransform;

enum class FieldType
{
	BOOL, INT, FLOAT, FLOAT_RX, FLOAT_RY, FLOAT_RZ, VECTOR3, VECTOR3INT, VECTOR2, VECTOR2INT, COLOR
};

struct FieldInfo
{
	wstring name;
	FieldType type;
	void* ptr;
	wstring filePath;
};

class ENGINE_DLL CComponent abstract
	: public UObject
{
protected:
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
	const _bool IsEnable() { return m_bEnable; };
	void SetObject(CGameObject* _obj) { m_pGameObject = _obj; }
	CTransform& getTransform();

protected:
	CGameObject* m_pGameObject;
	_bool m_bEnable;
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	_bool m_bClone;

public:
	virtual vector<FieldInfo> GetInspectorFields() PURE;

protected:
	inline FieldType DetectFieldType(bool&) { return FieldType::BOOL; }
	inline FieldType DetectFieldType(int&) { return FieldType::INT; }
	inline FieldType DetectFieldType(float&) { return FieldType::FLOAT; }
	inline FieldType DetectFieldType(vector2&) { return FieldType::VECTOR2; }
	inline FieldType DetectFieldType(vector2Int&) { return FieldType::VECTOR2INT; }
	inline FieldType DetectFieldType(vector3&) { return FieldType::VECTOR3; }
	//inline FieldType DetectFieldType(vector3Int&) { return FieldType::VECTOR3INT; }
	inline FieldType DetectFieldType(ColorValue&) { return FieldType::COLOR; }
};

END
