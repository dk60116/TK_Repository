#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
	friend class CGameObject;

	typedef struct TransformDirections
	{
		vector3 forward = vector3::zero();
		vector3 back = vector3::zero();
		vector3 left = vector3::zero();
		vector3 right = vector3::zero();
		vector3 up = vector3::zero();
		vector3 down = vector3::zero();
	}DIRECTIONS;

protected:
	explicit CTransform();
	~CTransform();

private:
	static CTransform* Create();

public:
	HRESULT Initialize() override;
	void Update() override;

public:
	CTransform* Get_Parent();
	void Set_Parent(CTransform* _parent);
	const list<CTransform*>& Get_ChldLIst() const;
	const DIRECTIONS& Get_Directions();
	const _matrix& Get_WorldMatrix() const;
	const _matrix Get_InverseWorldMatrix() const;

public:
	vector3 Get_Position() const;
	vector3 Get_LocalPosition() const;

	const vector3& Get_EulerAngles();
	const vector3& Get_LocalEulerAngles();

public:
	void Set_Position(const vector3& _pos);
	void Set_Position(const _float _x, const _float _y, const _float _z);
	void Set_PositionX(const _float _x);
	void Set_PositionY(const _float _y);
	void Set_PositionZ(const _float _z);

	void Set_LocalPosition(const vector3& _pos);
	void Set_LocalPosition(const _float _x, const _float _y, const _float _z);
	void Set_LocalPositionX(const _float _x);
	void Set_LocalPositionY(const _float _y);
	void Set_LocalPositionZ(const _float _z);

	void Add_Position(const vector3& _value);
	void Add_Position(const _float _x, const _float _y, const _float _z);
	void Add_PositionX(const _float _value);
	void Add_PositionY(const _float _value);
	void Add_PositionZ(const _float _value);

	void Set_Quaternion(const quaternion& _value);
	void Set_LocalQuaternion(const quaternion& _value);

	void Set_EulerAngle(const vector3& _rot);
	void Set_EulerAngle(const _float _x, const _float _y, const _float _z);
	void Set_EulerAngleX(const _float _x);
	void Set_EulerAngleY(const _float _y);
	void Set_EulerAngleZ(const _float _z);

	void Add_EulerAngle(const vector3& _rot);
	void Add_EulerAngle(const _float _x, const _float _y, const _float _z);
	void Add_EulerAngleX(const _float _value);
	void Add_EulerAngleY(const _float _value);
	void Add_EulerAngleZ(const _float _value);

	void Set_LocalEulerAngle(const vector3& _rot);
	void Set_LocalEulerAngle(const _float _x, const _float _y, const _float _z);
	void Set_LocalEulerAngleX(const _float _x);
	void Set_LocalEulerAngleY(const _float _y);
	void Set_LocalEulerAngleZ(const _float _z);

	void Add_LocalEulerAngle(const vector3& _rot);
	void Add_LocalEulerAngle(const _float _x, const _float _y, const _float _z);
	void Add_LocalEulerAngleX(const _float _value);
	void Add_LocalEulerAngleY(const _float _value);
	void Add_LocalEulerAngleZ(const _float _value);

	void Set_LocalScale(const vector3& _scale);
	void Set_LocalScale(const _float _x, const _float _y, const _float _z);
	void Set_LocalScaleX(const _float _value);
	void Set_LocalScaleY(const _float _value);
	void Set_LocalScaleZ(const _float _value);

private:
	void Bind_Matrix();
	void Bind_Direction();

private:
	_bool m_bIsRootParent;
	CTransform* m_pParent;
	list<CTransform*> m_lChildList;
	vector3 m_vPosition, m_vEulerAngles, m_vScale;
	vector3 m_vLocalPosition, m_vLocalEulerAngles;
	quaternion m_vQuaternion, m_vLocalQuaternion;
	_matrix m_vMatWorld, m_vMatLocalRotation;
	DIRECTIONS m_sDirections;
};

NS_END

