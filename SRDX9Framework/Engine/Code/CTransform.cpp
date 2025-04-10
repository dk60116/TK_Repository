#include "CTransform.h"
#include "CDebug.h"

CTransform::CTransform()
	: m_pParent(nullptr)
	, m_v3Position(vector3::zero())
	, m_v3Scale(vector3::one())
	, m_v3EulerAngles(vector3::zero())
	, m_v4Quaternion(quaternion::identity())
	, m_matWorld()
	, m_sDirections({})
{
}

CTransform::~CTransform()
{
}

void CTransform::Awake()
{
	CComponent::Awake();

	D3DXMatrixIdentity(&m_matWorld);
}

void CTransform::Start()
{
}

void CTransform::Update()
{
	CComponent::Update();

	UpdateWorld();
	UpdateDirections();
}

void CTransform::FixedUpdate()
{
}

void CTransform::LateUpdate()
{
}

void CTransform::Render()
{
}

void CTransform::OnEnable()
{
}

void CTransform::OnDisable()
{
}

void CTransform::OnDestroy()
{
	CComponent::OnDestroy();
}

void CTransform::UpdateWorld()
{
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, m_v3Scale.x, m_v3Scale.y, m_v3Scale.z);

	D3DXMATRIX matRotation;
	D3DXQUATERNION quat = m_v4Quaternion;
	D3DXMatrixRotationQuaternion(&matRotation, &quat);

	D3DXMATRIX matTranslation;
	D3DXMatrixTranslation(&matTranslation, m_v3Position.x, m_v3Position.y, m_v3Position.z);

	m_matWorld = matScale * matRotation * matTranslation;

	if (m_pParent)
	{
		m_matWorld *= m_pParent->m_matWorld;
	}
}

void CTransform::UpdateDirections()
{
	D3DXMATRIX rotMatrix;
	D3DXQUATERNION quat = m_v4Quaternion;
	D3DXMatrixRotationQuaternion(&rotMatrix, &quat);

	const D3DXVECTOR3 forward(0, 0, 1);
	const D3DXVECTOR3 up(0, 1, 0);
	const D3DXVECTOR3 right(1, 0, 0);

	D3DXVECTOR3 n_fwd, n_up, n_right;
	D3DXVec3TransformNormal(&n_fwd, &forward, &rotMatrix);
	D3DXVec3TransformNormal(&n_up, &up, &rotMatrix);
	D3DXVec3TransformNormal(&n_right, &right, &rotMatrix);

	m_sDirections.forward = vector3(n_fwd);
	m_sDirections.up = vector3(n_up);
	m_sDirections.right = vector3(n_right);

	m_sDirections.back = -m_sDirections.forward;
	m_sDirections.down = -m_sDirections.up;
	m_sDirections.left = -m_sDirections.right;
}

void CTransform::SetPosition(const vector3 _pos)
{
	m_v3Position = _pos;
}

void CTransform::SetPosition(const _float _x, const _float _y, const _float _z)
{
	m_v3Position = vector3(_x, _y, _z);
}

void CTransform::AddPosition(const vector3 _pos)
{
	m_v3Position += _pos;
}

void CTransform::AddPosition(const _float _x, const _float _y, const _float _z)
{
	m_v3Position += vector3(_x, _y, _z);
}

void CTransform::SetPositionX(const _float _x)
{
	m_v3Position.x = _x;
}

void CTransform::SetPositionY(const _float _y)
{
	m_v3Position.y = _y;
}

void CTransform::SetPositionZ(const _float _z)
{
	m_v3Position.z = _z;
}

void CTransform::AddPositionX(const _float _x)
{
	m_v3Position.x += _x;
}

void CTransform::AddPositionY(const _float _y)
{
	m_v3Position.y += _y;
}

void CTransform::AddPositionZ(const _float _z)
{
	m_v3Position.z += _z;
}

void CTransform::SetScale(const vector3 _scale)
{
	m_v3Scale = _scale;
}

void CTransform::SetScale(const _float _x, const _float _y, const _float _z)
{
	m_v3Scale = vector3(_x, _y, _z);
}

void CTransform::AddScale(const vector3 _scale)
{
	m_v3Scale += _scale;
}

void CTransform::AddScale(const _float _x, const _float _y, const _float _z)
{
	m_v3Scale += vector3(_x, _y, _z);
}

void CTransform::SetScaleX(const _float _x)
{
	m_v3Scale.x = _x;
}

void CTransform::SetScaleY(const _float _y)
{
	m_v3Scale.y = _y;
}

void CTransform::SetScaleZ(const _float _z)
{
	m_v3Scale.z = _z;
}

void CTransform::AddScaleX(const _float _x)
{
	m_v3Scale.x += _x;
}

void CTransform::AddScaleY(const _float _y)
{
	m_v3Scale.y += _y;
}

void CTransform::AddScaleZ(const _float _z)
{
	m_v3Scale.z += _z;
}

void CTransform::SetEulerAngles(const vector3 _rot)
{
	m_v3EulerAngles = _rot;
	m_v4Quaternion = quaternion::from_euler(m_v3EulerAngles);
}

void CTransform::SetEulerAngles(const _float _x, const _float _y, const _float _z)
{
	m_v3EulerAngles = vector3(_x, _y, _z);
	m_v4Quaternion = quaternion::from_euler(m_v3EulerAngles);
}

void CTransform::SetEulerAnglesX(const _float _x)
{
	m_v3EulerAngles.x = _x;
	m_v4Quaternion = quaternion::from_euler(m_v3EulerAngles);
}

void CTransform::SetEulerAnglesY(const _float _y)
{
	m_v3EulerAngles.y = _y;
	m_v4Quaternion = quaternion::from_euler(m_v3EulerAngles);
}

void CTransform::SetEulerAnglesZ(const _float _z)
{
	m_v3EulerAngles.z = _z;
	m_v4Quaternion = quaternion::from_euler(m_v3EulerAngles);
}

void CTransform::AddEulerAnglesX(const _float _x)
{
	m_v3EulerAngles.x += _x;
	m_v4Quaternion = quaternion::from_euler(m_v3EulerAngles);
}

void CTransform::AddEulerAnglesY(const _float _y)
{
	m_v3EulerAngles.y += _y;
	m_v4Quaternion = quaternion::from_euler(m_v3EulerAngles);
}

void CTransform::AddEulerAnglesZ(const _float _z)
{
	m_v3EulerAngles.z += _z;
	m_v4Quaternion = quaternion::from_euler(m_v3EulerAngles);
}

void CTransform::SetQuaternion(const quaternion _q)
{
	m_v4Quaternion = _q;
}

void CTransform::AddLocalXAxis(_float _angle)
{
	_float angle_rad = D3DXToRadian(_angle);

	D3DXQUATERNION xRot;
	D3DXVECTOR3 euler = vector3::right().vector();
	D3DXQuaternionRotationAxis(&xRot, &euler, angle_rad);

	m_v4Quaternion = quaternion(xRot) * m_v4Quaternion;
}

void CTransform::AddLocalYAxis(_float _angle)
{
	_float angle_rad = D3DXToRadian(_angle);

	D3DXQUATERNION yRot;
	D3DXVECTOR3 euler = vector3::up().vector();
	D3DXQuaternionRotationAxis(&yRot, &euler, angle_rad);

	m_v4Quaternion = quaternion(yRot) * m_v4Quaternion;
}

void CTransform::AddLocalZAxis(_float _angle)
{
	_float angle_rad = D3DXToRadian(_angle);

	D3DXQUATERNION zRot;
	D3DXVECTOR3 euler = vector3::forward().vector();
	D3DXQuaternionRotationAxis(&zRot, &euler, angle_rad);

	m_v4Quaternion = quaternion(zRot) * m_v4Quaternion;
}

void CTransform::RotateLocalAxis(const vector3 local_axis, _float angle_deg)
{
	_float angle_rad = D3DXToRadian(angle_deg);

	// 1. 로컬 축 → 월드 축 변환 (현재 회전에 기반)
	D3DXMATRIX rotMatrix;
	D3DXQUATERNION q = m_v4Quaternion;
	D3DXMatrixRotationQuaternion(&rotMatrix, &q);

	D3DXVECTOR3 local = local_axis; // vector3 → D3DXVECTOR3
	D3DXVECTOR3 axis_world;
	D3DXVec3TransformNormal(&axis_world, &local, &rotMatrix);

	// 2. 축 기반 회전 쿼터니언 생성
	D3DXQUATERNION qRot;
	D3DXQuaternionRotationAxis(&qRot, &axis_world, angle_rad);

	// 3. 쿼터니언 누적 (로컬 기준 회전)
	m_v4Quaternion = quaternion(qRot) * m_v4Quaternion;
}

void CTransform::AddEulerAngles(const vector3 _rot)
{
	m_v3EulerAngles += _rot;
	m_v4Quaternion = quaternion::from_euler(m_v3EulerAngles);
}

void CTransform::AddEulerAngles(const _float _x, const _float _y, const _float _z)
{
	m_v3EulerAngles += vector3(_x, _y, _z);
	m_v4Quaternion = quaternion::from_euler(m_v3EulerAngles);
}

void CTransform::LookAt(vector3 _target)
{
	vector3 dir = (_target - m_v3Position).normalized();

	_float pitch = asinf(-dir.y);
	_float yaw = atan2f(dir.x, dir.z);
	_float roll = 0.0f;

	vector3 euler = vector3(D3DXToDegree(pitch), D3DXToDegree(yaw), D3DXToDegree(roll));

	SetEulerAngles(euler);
}

void CTransform::LookAt(CTransform& _target)
{
	LookAt(_target.getTransform().getPosition());
}

void CTransform::LookAt(vector3 _target, vector3 _front)
{
	vector3 world_target_dir = (_target - m_v3Position).normalized();

	_matrix rotMatrix;
	D3DXVECTOR3 vFrom = _front.vector();
	D3DXVECTOR3 vTo = world_target_dir.vector();

	D3DXQUATERNION qRot;
	D3DXVECTOR3 upVector = vector3::up();
	D3DXQuaternionRotationAxis(&qRot, &upVector, 0);

	if (D3DXVec3LengthSq(&vFrom) > 0.0001f && D3DXVec3LengthSq(&vTo) > 0.0001f)
	{
		D3DXVec3Normalize(&vFrom, &vFrom);
		D3DXVec3Normalize(&vTo, &vTo);

		D3DXVECTOR3 axis;
		D3DXVec3Cross(&axis, &vFrom, &vTo);
		_float angle = acosf(D3DXVec3Dot(&vFrom, &vTo));

		if (fabs(angle) > 0.0001f)
		{
			D3DXQuaternionRotationAxis(&qRot, &axis, angle);
		}
	}

	m_v4Quaternion = quaternion(qRot);
}

void CTransform::LookAt(CTransform& _target, vector3 _front)
{
}
