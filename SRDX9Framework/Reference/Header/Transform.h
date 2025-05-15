#pragma once

#include "Component.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CGameObject;

struct Directions
{
	vector3 forward = vector3::zero();
	vector3 back = vector3::zero();
	vector3 left = vector3::zero();
	vector3 right = vector3::zero();
	vector3 up = vector3::zero();
	vector3 down = vector3::zero();
};

class ENGINE_DLL CTransform final
	: public CComponent
{
private:
	explicit CTransform();
	~CTransform();

public:
	void Init() override;
	void Awake() override;
	void Start() override;
	void UpdateEditor() override;
	void Update() override;
	void FixedUpdate() override;
	void LateUpdate() override;
	void Render() override;
	void OnEnable() override;
	void OnDisable() override;
	void OnDestroy() override;

public:
	static CTransform* Create();

private:
	void UpdateWorld();
	void UpdateDirections();

public:
	const wstring getName();
	CTransform* getParent() const { return m_pParent; }
	void SetParent(CTransform* _parent);
	const _bool isRoot() const { return m_bIsRootParent; }
	list<CTransform*>& getChilds() { return m_lChildList;; }

	vector3& getPosition() { return m_vWorldPos; };
	vector3& getEulerAngles() { return m_vEulerAngles; };

	vector3& getLocalPosition() { return m_vPosition; }
	vector3& getLocalScale() { return m_vScale; }
	vector3& getLocalEulerAngles() { return m_vEulerAngles; }

	void SetPosition(const vector3 world_pos);
	void SetPosition(const _float _x, const _float _y, const _float _z);
	void AddPosition(const vector3 _world_delta);
	void AddPosition(const _float _x, const _float _y, const _float _z);
	void SetPositionX(const _float _x);
	void SetPositionY(const _float _y);
	void SetPositionZ(const _float _z);
	void AddPositionX(const _float _x);
	void AddPositionY(const _float _y);
	void AddPositionZ(const _float _z);

	void EditRotation(const FieldType _xyz, const _float _delta);

	void SetEulerAngles(const vector3 _world_euler_deg);
	void SetEulerAngles(const _float _x, const _float _y, const _float _z);
	void AddEulerAngles(const vector3 _delta);
	void AddEulerAngles(const _float _x, const _float _y, const _float _z);
	void SetEulerAnglesX(const _float _x);
	void SetEulerAnglesY(const _float _y);
	void SetEulerAnglesZ(const _float _z);
	void AddEulerAnglesX(const _float _x);
	void AddEulerAnglesY(const _float _y);
	void AddEulerAnglesZ(const _float _z);

	void SetLocalPosition(const vector3& _pos);
	void SetLocalPosition(const _float _x, const _float _y, const _float _z);
	void AddLocalPosition(const vector3& _pos);
	void AddLocalPosition(const _float _x, const _float _y, const _float _z);
	void SetLocalPositionX(const _float _x);
	void SetLocalPositionY(const _float _y);
	void SetLocalPositionZ(const _float _z);
	void AddLocalPositionX(const _float _x);
	void AddLocalPositionY(const _float _y);
	void AddLocalPositionZ(const _float _z);
	
	void SetLocalScale(const vector3& _scale);
	void SetLocalScale(const _float _x, const _float _y, const _float _z);
	void SetLocalScale(const _float _value);
	void AddLocalScale(const vector3& _scale);
	void AddLocalScale(const _float _x, const _float _y, const _float _z);
	void SetLocalScaleX(const _float _x);
	void SetLocalScaleY(const _float _y);
	void SetLocalScaleZ(const _float _z);
	void AddLocalScaleX(const _float _x);
	void AddLocalScaleY(const _float _y);
	void AddLocalScaleZ(const _float _z);

	void SetLocalEulerAngles(const vector3& _rot);
	void SetLocalEulerAngles(const _float _x, const _float _y, const _float _z);
	void AddLocalEulerAngles(const vector3& _rot);
	void AddLocalEulerAngles(const _float _x, const _float _y, const _float _z);
	void SetLocalEulerAnglesX(const _float _x);
	void SetLocalEulerAnglesY(const _float _y);
	void SetLocalEulerAnglesZ(const _float _z);
	void AddLocalEulerAnglesX(const _float _x);
	void AddLocalEulerAnglesY(const _float _y);
	void AddLocalEulerAnglesZ(const _float _z);

	void SetQuaternion(const quaternion& _q);
	void AddLocalXAxis(_float _angle);
	void AddLocalYAxis(_float _angle);
	void AddLocalZAxis(_float _angle);
	void RotateLocalAxis(const vector3& local_axis, _float angle_deg);

	void LookAt(const vector3 _target);
	void LookAt(CTransform& _target);
	void LookAt(const vector3 _target, const vector3 _front);
	const _matrix& getWorldMatrix() const { return m_matWorld; }
	const Directions& getDirections() const { return m_sDirections; }

public:
	void Serialize(CSerialzer& _s) override;

private:
	_bool m_bIsRootParent;
	CTransform* m_pParent;
	list<CTransform*> m_lChildList;
	vector3 m_vPosition, m_vScale, m_vRotation;
	vector3 m_vEulerAngles;
	vector3 m_vWorldPos, m_vWorldEulerAngles;
	quaternion m_vQuaternion, m_vWorldQuaternion;
	_matrix m_matWorld;
	Directions m_sDirections;

	BEGIN_SERIALIZEFIELD
		SERIALIZEFIELD(m_vPosition)
		SERIALIZEFIELD(m_vRotation)
		SERIALIZEFIELD(m_vScale)
	END_SERIALIZEFIELD
};

END

