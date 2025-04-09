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
	void SetScale(const vector3 _scale);
	void SetScale(const _float _x, const _float _y, const _float _z);
	void AddPosition(const vector3 _pos);
	void SetRotation(const vector3 _rot);
	void SetRotation(const _float _x, const _float _y, const _float _z);
	void AddRotation(const vector3 _rot);
	void LookAt(vector3 _target);
	void LookAt(CTransform& _target);
	const _matrix& getWorldMatrix() const { return m_matWorld; }
	const Directions& getDirections() const { return m_sDirections; }

private:
	CTransform* m_pParent;
	vector3 m_v3Position, m_v3Scale, m_v3EulerAngles;
	_matrix m_matWorld;
	Directions m_sDirections;
};

END

