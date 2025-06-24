#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
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

public:
	static CTransform* Create();

public:
	HRESULT Initialize() override;
	void Update() override;

public:
	const DIRECTIONS& Get_Direction();
	const _matrix& Get_WorldMatrix() const;
	const _matrix Get_InverseWorldMatrix() const;

public:
	vector3 Get_Position() const;
	vector3 Get_LocalPosition() const;

public:
	void Set_Position(const vector3& _pos);
	void Set_Position(const _float _x, const _float _y, const _float _z);
	void Set_PositionX(const _float _x);
	void Set_PositionY(const _float _y);
	void Set_PositionZ(const _float _z);

private:
	void Bind_Matrix();
	void Bind_Direction();

private:
	_bool m_bIsRootParent;
	CTransform* m_pParent;
	list<CTransform*> m_lChildList;
	vector3 m_vPosition, m_vRotation, m_vScale;
	vector3 m_vEulerAngles;
	vector3 m_vLocalPosition, m_vLocalEulerAngles;
	quaternion m_vQuaternion, m_vLocalQuaternion;
	_matrix m_vMatWorld, m_vMatLocalRotation;
	DIRECTIONS m_sDirections;
};

NS_END

