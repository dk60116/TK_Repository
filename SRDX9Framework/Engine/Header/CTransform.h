#pragma once

#include "CComponent.h"
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

class ENGINE_DLL CTransform
	: public CComponent
{
public:
	explicit CTransform();
	~CTransform();

public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void FixedUpdate() override;
	void LateUpdate() override;
	void Render() override;
	void OnEnable() override;
	void OnDisable() override;
	void OnDestroy() override;

private:
	void UpdateWorld();
	void UpdateDirections();

public:
	void SetParent(CTransform& _parent) { m_pParent = &_parent; }
	vector3& getPosition() { return m_v3Position; }
	vector3& getScale() { return m_v3Scale; }
	vector3& getEulerAngle() { return m_v3EulerAngles; }

	void SetPosition(const vector3 _pos);
	void SetPosition(const _float _x, const _float _y, const _float _z);
	void AddPosition(const vector3 _pos);
	void AddPosition(const _float _x, const _float _y, const _float _z);
	void SetPositionX(const _float _x);
	void SetPositionY(const _float _y);
	void SetPositionZ(const _float _z);
	void AddPositionX(const _float _x);
	void AddPositionY(const _float _y);
	void AddPositionZ(const _float _z);
	
	void SetScale(const vector3 _scale);
	void SetScale(const _float _x, const _float _y, const _float _z);
	void AddScale(const vector3 _scale);
	void AddScale(const _float _x, const _float _y, const _float _z);
	void SetScaleX(const _float _x);
	void SetScaleY(const _float _y);
	void SetScaleZ(const _float _z);
	void AddScaleX(const _float _x);
	void AddScaleY(const _float _y);
	void AddScaleZ(const _float _z);

	void SetEulerAngles(const vector3 _rot);
	void SetEulerAngles(const _float _x, const _float _y, const _float _z);
	void AddEulerAngles(const vector3 _rot);
	void AddEulerAngles(const _float _x, const _float _y, const _float _z);
	void SetEulerAnglesX(const _float _x);
	void SetEulerAnglesY(const _float _y);
	void SetEulerAnglesZ(const _float _z);
	void AddEulerAnglesX(const _float _x);
	void AddEulerAnglesY(const _float _y);
	void AddEulerAnglesZ(const _float _z);

	void SetQuaternion(const quaternion _q);
	void AddLocalXAxis(_float _angle);
	void AddLocalYAxis(_float _angle);
	void AddLocalZAxis(_float _angle);
	void RotateLocalAxis(const vector3 local_axis, _float angle_deg);

	void LookAt(vector3 _target);
	void LookAt(CTransform& _target);
	const _matrix& getWorldMatrix() const { return m_matWorld; }
	const Directions& getDirections() const { return m_sDirections; }

private:
	CTransform* m_pParent;
	vector3 m_v3Position, m_v3Scale, m_v3EulerAngles;
	quaternion m_v4Quaternion;
	_matrix m_matWorld;
	Directions m_sDirections;
};

END

