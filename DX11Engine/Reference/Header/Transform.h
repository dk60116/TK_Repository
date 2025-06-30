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
	void OnDestroy() override;

public:
	CTransform* Get_Parent();
	void Set_Parent(CTransform* _parent);
	void Set_Parent(CGameObject* _parentObj);
	CTransform* Get_Child(const _int _index);
	CTransform* Find_Child(wstring _name);
	const list<CTransform*>& Get_ChldList() const;
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

	void Add_Quaternion(const quaternion& _delta);

	void Set_EulerAngles(const vector3& _rot);
	void Set_EulerAngles(const _float _x, const _float _y, const _float _z);
	void Set_EulerAnglesX(const _float _x);
	void Set_EulerAnglesY(const _float _y);
	void Set_EulerAnglesZ(const _float _z);

	void Add_EulerAngles(const vector3& _rot);
	void Add_EulerAngles(const _float _x, const _float _y, const _float _z);
	void Add_EulerAnglesX(const _float _value);
	void Add_EulerAnglesY(const _float _value);
	void Add_EulerAnglesZ(const _float _value);

	void Set_LocalEulerAngles(const vector3& _rot);
	void Set_LocalEulerAngles(const _float _x, const _float _y, const _float _z);
	void Set_LocalEulerAnglesX(const _float _x);
	void Set_LocalEulerAnglesY(const _float _y);
	void Set_LocalEulerAnglesZ(const _float _z);

	void Add_LocalEulerAngles(const vector3& _rot);
	void Add_LocalEulerAngles(const _float _x, const _float _y, const _float _z);
	void Add_LocalEulerAnglesX(const _float _value);
	void Add_LocalEulerAnglesY(const _float _value);
	void Add_LocalEulerAnglesZ(const _float _value);

	void Set_LocalScale(const vector3& _scale);
	void Set_LocalScale(const _float _x, const _float _y, const _float _z);
	void Set_LocalScaleX(const _float _value);
	void Set_LocalScaleY(const _float _value);
	void Set_LocalScaleZ(const _float _value);

	void LookAt(const vector3& _target);

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

